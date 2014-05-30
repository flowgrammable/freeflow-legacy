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

namespace freeflow {

// Initialize the reactor.
inline
Reactor::Reactor()
  : running_(true), handlers_(), timers_(), expired_()
{
  expired_.reserve(32);
}

/// Register the handler with the reactor.
inline void
Reactor::add_handler(Event_handler* h) { 
  handlers_.add(h); 
}

/// Unregister the handler with the reactor. Any outstanding timers
/// are canceled before removing the handler. If the handler is managed
/// by the reactor, then it is also deleted.
inline void
Reactor::remove_handler(Event_handler* h) { 
  timers_.cancel(h);
  handlers_.remove(h); 
  if (h->has_flags(HANDLER_IS_OWNED))
    delete h;
}

/// Remove all event handlers from the reactor after it has stopped.
inline void
Reactor::remove_handlers() {
  for (Event_handler* h : handlers_)
    if (h) remove_handler(h);
}

/// Dynamically create a new handler of type T, constructed with the
/// given arguments. The new handler is subscribed to lifetime events,
/// allowing the reactor to manage its lifetime.
template<typename T, typename... Args>
  inline T*
  Reactor::new_handler(Args&&... args) {
    T* h = new T(*this, std::forward<Args>(args)...);
    new_handler(h);
    return h;
  }

/// Register the new handler with the reactor. The reactor assumes
/// ownership of the handler.
inline void
Reactor::new_handler(Event_handler* h) {
  h->set_flags(HANDLER_IS_OWNED);
  handlers_.add(h);
}

/// Subscribe a handler to the specified events.
inline void
Reactor::subscribe_events(Event_handler* h, Event_mask m) {
  handlers_.subscribe(h, m);
}

/// Unsubscribe a handler from the specified events.
inline void
Reactor::unsubscribe_events(Event_handler* h, Event_mask m) {
  handlers_.unsubscribe(h, m);
}

/// Schedule a timer for the handler. The handler must be registered
/// with the reactor.
inline void
Reactor::schedule_timer(Event_handler* h, int id, Microseconds us) {
  timers_.schedule(h, id, us);
}

/// Rechedule a timer for the handler. 
inline void
Reactor::reschedule_timer(Event_handler* h, int id, Microseconds us) {
  timers_.reschedule(h, id, us);
}

/// Cancel all timers associated with the handler.
inline void
Reactor::cancel_timer(Event_handler* h, int id) {
  timers_.cancel(h, id);
}

/// Stop the reactor from running.
inline void
Reactor::stop() { running_ = false; }


} // namesapce freeflow
