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

#include "handler.hpp"

namespace freeflow {

/// Insert a new handler into the registry. The handler is directly mapped
/// by its file descriptor. Behavior is undefined a handler is already
/// registerd for that file descriptor. 
///
/// The handler's open() method is called when the service is added. If
/// open() returns false, the handler is not added, and the function
/// returns false.
void
Handler_registry::add(Event_handler* h) {
  assert(0 <= h->fd() and h->fd() < FD_SETSIZE);
  assert(reg_[h->fd()] == nullptr);
  
  // Insert the handler.
  reg_[h->fd()] = h;
  max_ = std::max(h->fd(), max_);
  
  // Update internal tables based on the handlers initial subscription.
  on_subscribe(h);

  // Notify the handler that it is now active.
  h->on_open();
}

/// Remove the service hander. The handler's close() method is called
/// when the handler is removed.
///
/// This function always returns true.
///
/// \todo If active falls below some some low water mark, re-scan the
/// list for a new max value. It's not strictly necessary, since *nixes
/// recycle fds, but if active is very low and max is very high, then
/// we're going to have a very sparse list.
void
Handler_registry::remove(Event_handler* h) {
  assert(0 <= h->fd() and h->fd() < FD_SETSIZE);

  // Notify the handler that it will soon be inactive.
  h->on_close();
  
  // Update internal tables based on the handler's current subscriptions,
  // but do not modfiy the handler.
  on_unsubscribe(h);
  
  // Remove the handler.
  reg_[h->fd()] = {};
}

/// Update the handler's event mask by registering it to receive
/// the indicated events.
///
/// \todo Add support for signals.
void
Handler_registry::subscribe(Event_handler* h, Event_mask m) {
  assert(reg_[h->fd()] == h);
  h->subscribe(m);  
  on_subscribe(h);
}

/// Update the handler's event mask by unregistering it from various
/// events.
void
Handler_registry::unsubscribe(Event_handler* h, Event_mask m) {
  assert(reg_[h->fd()] == h);
  h->unsubscribe(m);
  on_unsubscribe(h);
}

/// Update internal tables based on subscriptions.
void
Handler_registry::on_subscribe(Event_handler* h) {
  if (h->events() & READ_EVENTS)
    wait_.read.insert(h->fd());
  if (h->events() & WRITE_EVENTS)
    wait_.write.insert(h->fd());
  if (h->events() & EXCEPT_EVENTS)
    wait_.except.insert(h->fd());
}

/// Update internal tables based on subscriptions.
void
Handler_registry::on_unsubscribe(Event_handler* h) {
  if (h->events() & READ_EVENTS)
    wait_.read.remove(h->fd());
  if (h->events() & WRITE_EVENTS)
    wait_.write.remove(h->fd());
  if (h->events() & EXCEPT_EVENTS)
    wait_.except.remove(h->fd());
}

} // namespace freeflow
