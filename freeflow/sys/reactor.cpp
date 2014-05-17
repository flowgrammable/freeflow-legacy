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

namespace freeflow {
namespace {

// Returns true iff the event handler is subscibed to events m.
inline bool
should_notify(Event_handler* h, Event_mask m) {
  return h->is_subscribed(m);
}

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

void
notify_timer(const Timer& t, Resource_set& close) {
  Event_handler* h = t.handler;
  if (should_notify(h, TIME_EVENTS))
    if (not h->on_time(t.id))
      close.insert(h->fd());
}

} // namespace

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
void
Reactor::close_handlers(const Resource_set& close) {
   for (Event_handler* h : handlers_) {
    if (h and close.test(h->fd()))
      remove_handler(h);
  }
}



/// Run the reactor's event loop until stoppage is indicated.
void
Reactor::run() {
  running_ = true;
  while (running_) {
    // Select on the registered handlers. Only wait 10 ms for
    // an event to trigger.
    Select_set dispatch = handlers_.wait();
    Selector s(handlers_.max() + 1, dispatch);    
    s(10_ms);

    // Close any handlers that need to removed from the
    // registry and closed.
    Resource_set close;
    notify_select(dispatch, close);
    notify_timers(close);

    // Close any outstanding handlers
    close_handlers(close);
  }
}

} // namespace freeflow
