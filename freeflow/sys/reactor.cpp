// Copyright (c) 2013-2014 Flowgrammable.org
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an "AS IS"
// BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing
// permissions and limitations under the License.

#include "reactor.hpp"
#include "signal.hpp"

namespace freeflow {
namespace {

// -------------------------------------------------------------------------- //
// Event notification

// Returns true iff the event handler is subscibed to events m.
inline bool
should_notify(Event_handler* h, Event_mask m) { return h->is_subscribed(m); }

// Returns true iff  the event handler h is subscribed to events m
// and that event is indicated in the resource set r.
inline bool
should_notify(Event_handler* h, Event_mask m, const Resource_set& r) {
  return h->is_subscribed(m) and r.test(h->fd());
}


// Notify the handler that data is available for reading. If the handler
// returns false, register the handler for closing.
inline void
notify_read(Event_handler* h, const Resource_set& rs, Resource_set& close) {
  if (should_notify(h, READ_EVENTS, rs))
    if (not h->on_read())
      close.insert(h->fd());
}

// Notify the handler it is possible to send data. If the handler
// returns false, register the handler for closing.
inline void
notify_write(Event_handler* h, const Resource_set& rs, Resource_set& close) {
  if (should_notify(h, WRITE_EVENTS, rs))
    if (not h->on_write())
      close.insert(h->fd());
}

// Notify the hander that urgent data is available for reading. If the
// handler returns false, register the handler for closing.
inline void
notify_except(Event_handler* h, const Resource_set& rs, Resource_set& close) {
  if (should_notify(h, EXCEPT_EVENTS, rs))
    if (not h->on_except())
      close.insert(h->fd());
}

inline void
notify_timer(const Timer& t, Resource_set& close) {
  Event_handler* h = t.handler;
  if (should_notify(h, TIME_EVENTS))
    if (not h->on_time(t.id))
      close.insert(h->fd());
}

// -------------------------------------------------------------------------- //
// Signaling infrastructure

using Reactor_list = std::vector<Reactor*>;

/// A global list of reactor instances. This allos multiple reactors
/// to be running in a single thread or process.
static Reactor_list reactors_;

// The basic signal handler simply records the signal that
// was received by the process. This is acted on in the
// event loop.
//
// BUG: This is fundamentally broken. The handler needs to queue then
// signal for each reactor instance.
void
signal_handler(int sig) { 
  for (Reactor* r : reactors_)
    r->send_signal(sig);
}

} // namespace

// -------------------------------------------------------------------------- //
// Reactor initialization

namespace impl {

inline Reactor*
Reactor_init::self() { return reinterpret_cast<Reactor*>(this); }


// Initialize the signal handler infrastructure. Note that this
// happens only once.
Reactor_init::Reactor_init() {
  init_signals();

  // Add the reactor to the global reactor list.
  reactors_.push_back(self());
}

Reactor_init::~Reactor_init() {
  // Remove this reactor from the registry.
  // NOTE: This is not efficient, but we expect the number of reactors
  // to be small.
  reactors_.erase(std::find(reactors_.begin(), reactors_.end(), self()));
}


// One-time initialiation of signal handling.
///
/// \todo This should probably register handlers on a per-thread
/// basis, if that's possible.
///
/// \todo Consider specifying the set of handled signals via a
/// signal mask. We can simply iterate over that and install
/// (or uninstall) the handler where appropriate.
void 
Reactor_init::init_signals() {
  Signal_action sa;
  if (not sa) {
    sa = Signal_action(signal_handler);

    // TODO: What set of signals do I really want here? These are
    // commonly handled, but by no means the only viable set.
    //
    // NOTE: Do not handle signals that would cause the behavior or
    // the program to become undefined if ignored.
    sa.install(SIGHUP);
    sa.install(SIGINT);
    sa.install(SIGQUIT);
    sa.install(SIGTERM);
    sa.install(SIGUSR1);
    sa.install(SIGUSR2);
  }
}

} // namesspace impl


// -------------------------------------------------------------------------- //
// Reactor implementation

// Notify each handler that events are (or may be) available.
void
Reactor::notify_select(const Select_set& sel, Resource_set& close) {
  for (Event_handler* h : handlers_)
    if (h) {
      notify_read(h, sel.read, close);
      notify_write(h, sel.write, close);
      notify_except(h, sel.except, close);
    }
}

// Dequeue expired timers and notify event handlers.
void
Reactor::notify_timers(Resource_set& close) {
  // Dqueue timers relative to the current time.
  Time_point t = now();
  while (timers_.expired(t))
    expired_.push_back(timers_.expire());

  // Notify the timer's corresponding event handler.
  for (Timer& t : expired_)
    notify_timer(t, close);

  // Reset the expiry list.
  expired_.clear();
}

/// If the handler is is in the close set, remove it from the reactor.
///
/// \todo Provide a mechanism for testing to see if any elements of
/// a resource set are in fact set (i.e., the value is non-zero). If
/// no handlers are set, don't iterate over handlers.
void
Reactor::close_handlers(const Resource_set& close) {
  for (Event_handler* h : handlers_) {
    if (h and close.test(h->fd()))
      remove_handler(h);
  }
}

// Notify each handler of each pending signal, and then clear the queue.
inline void
Reactor::notify_signal(Resource_set& close) {
  for (int s : signals_)
    for (Event_handler* h : handlers_) {
      if (h and h->is_subscribed(SIGNAL_EVENTS))
        if (not h->on_signal(s))
          close.insert(h->fd());
    }
  signals_.clear();
}

/// Run the reactor's event loop until stoppage is indicated.
void
Reactor::run() {
  // FIXME: This granularity of time slice is probably not right.
  running_ = true;
  while (running_)
    run(10_ms);
  remove_handlers();
}

void
Reactor::run(Microseconds us) {
  Resource_set close;
  
  // Select any event handlers with active events. Only dispatch if
  // there were any events.
  Select_set dispatch = handlers_.wait();
  Selector select(handlers_.max() + 1, dispatch);    

  // Block all signals when calling select. This lets us proces them
  // after the select call instead of asynchronously. Don't block
  // signals that could result in undefined behavior.
  //
  // TODO: What if we block everything all the time, and simply
  // check pending signals as part of the event loop. That seems like
  // it might be more effective. It also means that none of our
  // blocking system calls will actually be interrupted.
  // Signal_set sigs = Signal_set::all();
  
  // Run select. Return when any event is detected, including signals.
  int n = select();

  // Notify handlers if signals are present. This is done 
  notify_signal(close);
  
  // Check for read/write/except notifications if select indicates
  // that some hav events.
  if (n)
    notify_select(dispatch, close);
  
  // Notify handlers of expired timers.
  notify_timers(close);

  // Close any outstanding handlers
  close_handlers(close);
}

} // namespace freeflow
