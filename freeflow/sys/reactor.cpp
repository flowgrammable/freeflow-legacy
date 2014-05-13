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
using Rset = Resource_set;
using Sset = Select_set;
using Hreg = Handler_registry;

// Notify the handler if it has a read event. If the handler indicates
// closure, add it to the close set. If the handler indicates
void
notify_read(Reactor& r, Handler* h, const Rset& rs, Rset& close) {
  if (rs.test(h->fd()))
    if (not h->on_read(r))
      close.insert(h->fd());
}

// Notify the handler if it has a write event. If the handler indicates
// closure, add it to the close set. If the handler indicates
void
notify_write(Reactor& r, Handler* h, const Rset& rs, Rset& close) {
  if (rs.test(h->fd()))
    if (not h->on_write(r))
      close.insert(h->fd());
}


// Notify handlers of events.
// TODO: Test error sets also.
void
notify_handlers(Reactor& r, Hreg& hr, const Sset& ss, Rset& close) {
  for (Handler* h : hr)
    if (h) {
      notify_read(r, h, ss.read, close);
      notify_write(r, h, ss.write, close);
    }
}

/// If the handler is is in the close set, remove it from the
/// registry.
void
close_handlers(Reactor& r, Hreg& hr, const Rset& close) {
   for (Handler* h : hr) {
    if (h and close.test(h->fd()))
      r.remove_handler(h);
  }
}
} // namespace


Reactor::Reactor() { }

Reactor::~Reactor() {
  for (Handler* h : handlers_) {
    if (h) remove_handler(h);
  }
}

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
    notify_handlers(*this, handlers_, dispatch, close);

    // Trigger any timers that may have expired.
    timers_.dispatch(*this);

    // Before exiting, close any outstanding handlers
    close_handlers(*this, handlers_, close);
  }
}

} // namespace freeflow
