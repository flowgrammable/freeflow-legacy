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

#include <iostream>

namespace freeflow {

// -------------------------------------------------------------------------- //
// Abstract handler

// Note that r may not be iniatilized.
inline
Event_handler::Event_handler(Reactor& r,int fd, Event_mask m)
  : react_(r),  events_(m), fd_(fd) { }

/// The on_close event is sent when the event handler is removed
/// from the reactor.
inline bool 
Event_handler::on_close() { return true; }

/// The on_read event is sent when data is available for reading.
inline bool
Event_handler::on_read() { return true; }

/// The on_write event is sent when it is possible to write data.
inline bool
Event_handler::on_write() { return true; }

/// The on_except event is sent when urgent (out-of-band) data is 
/// available for reading.
inline bool
Event_handler::on_except() { return true; }

/// The on_time event is sent when the timer, indicated by the given
/// timer-id was triggered.
inline bool
Event_handler::on_time(int) { return true; }

/// The on_signal event is sent when the given signal was caught by
/// the program.
inline bool
Event_handler::on_signal(int) { return true; }

inline Reactor&
Event_handler::reactor() { return react_; }

/// Returns the uderlying file descriptor.
inline int 
Event_handler::fd() const { return fd_; }

/// Returns the set of events subscribed to by the event handler.
inline Event_mask
Event_handler::events() const { return events_; }

/// Returns true if the event handler is subscribed to the given
/// events.
inline bool
Event_handler::is_subscribed(Event_mask m) const { return events_ & m; }

/// Subscribe the event handler to the given set of events. This is used
/// indirectly by the reactor to manage the handler. Do not use this
/// function.
inline void
Event_handler::subscribe(Event_mask m) { events_ |= m; }

/// Unsubscribe the event handler from the given set of events. This is
/// used indirectly by the reactor to manage the handler. Do not use
/// this function.
inline void
Event_handler::unsubscribe(Event_mask m) { events_ &= ~m; }

/// Returns the set of flags used to manage the event handler.
inline Handler_flags
Event_handler::flags() const { return flags_; }

inline bool
Event_handler::has_flags(Handler_flags f) const { return flags_ & f; }

/// Set management flags. This is called only by the reactor. Do not use
/// this function.
inline void
Event_handler::set_flags(Handler_flags f) { flags_ |= f; }

/// Clear management flags. This is called only be the reator. Do not
/// use this functions.
inline void
Event_handler::clear_flags(Handler_flags f) { flags_ &= ~f; }


// -------------------------------------------------------------------------- //
// Resource handler

template<typename T>
  template<typename... Args>
    inline
    Resource_wrapper<T>::Resource_wrapper(Args&&... args)
      : rc_(std::forward<Args>(args)...) { }

/// Initialize the resource handler for the given reactor, initially
/// subscroibe to the given events, and whose underlying resource is
/// constructed from the given arguments.
template<typename T>
  template<typename... Args>
    inline 
    Basic_event_handler<T>::Basic_event_handler(Reactor& r,
                                                Event_mask m,
                                                Args&&... args)
      : Resource_wrapper<T>(std::forward<Args>(args)...)
      , Event_handler(r, this->rc_.fd(), m)
    { }

template<typename T>
inline T&
Basic_event_handler<T>::rc() { return this->rc_; }

template<typename T>
inline const T& 
Basic_event_handler<T>::rc() const { return this->rc_; }

// -------------------------------------------------------------------------- //
// Registry

inline
Handler_registry::Handler_registry() 
  : reg_(FD_SETSIZE), max_(-1), wait_()
{ }

inline const Select_set&
Handler_registry::wait() const { return wait_; }

inline int
Handler_registry::max() const { return max_; }

inline Handler_registry::iterator
Handler_registry::begin() { return reg_.begin(); }

inline Handler_registry::iterator 
Handler_registry::end() { return reg_.end(); }

inline Handler_registry::const_iterator
Handler_registry::begin() const { return reg_.begin(); }

inline Handler_registry::const_iterator
Handler_registry::end() const { return reg_.end(); }

} // namespace nocontrol
