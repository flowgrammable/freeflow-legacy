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

namespace nocontrol {

/// Register the handler with the reactor.
inline void
Reactor::add_handler(Handler* h) { 
  handlers_.add(*this, h); 
}

/// Unregister the handler with the reactor. Any outstanding timers
/// are canceled before removing the handler.
inline void
Reactor::remove_handler(Handler* h) { 
  timers_.cancel(h);
  handlers_.remove(*this, h); 
}

/// Schedule a timer for the handler. The handler must be registered
/// with the reactor.
inline void
Reactor::schedule_timer(Handler* h, int id, ff::Microseconds us) {
  timers_.schedule(h, id, us);
}

/// Cancel all timers associated with the handler.
inline void
Reactor::cancel_timer(Handler* h, int id) {
  timers_.cancel(h, id);
}

/// Stop the reactor from running.
inline void
Reactor::stop() { running_ = false; }

} // namesapce nocontrol
