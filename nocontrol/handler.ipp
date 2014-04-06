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

namespace nocontrol {

// -------------------------------------------------------------------------- //
// Abstract handler

// Note that r may not be iniatilized.
inline
Handler::Handler(ff::Resource& r)
  : r_(r) { }

inline bool 
Handler::on_open(Reactor&) { return true; }

inline bool 
Handler::on_close(Reactor&) { return true; }

inline bool
Handler::on_read(Reactor&) { return true; }

inline bool
Handler::on_write(Reactor&) { return true; }

inline bool
Handler::on_error(Reactor&) { return true; }

inline bool
Handler::on_time(Reactor&, int) { return true; }

inline bool
Handler::on_signal(Reactor&) { return true; }

inline int 
Handler::fd() const { return r_.fd(); }

// -------------------------------------------------------------------------- //
// Resource handler

/// Initialize a resource handler by moving from the given resource.
template<typename T>
  inline 
  Resource_handler<T>::Resource_handler(T&& r)
    : Handler(rc_), rc_(std::move(r)) { }

/// Initialize the resource handler by constructing a socket from the
/// given arguments.
template<typename T>
  template<typename... Args>
    inline 
    Resource_handler<T>::Resource_handler(Args&&... args)
      : Handler(rc_), rc_(std::forward<Args>(args)...) { }

template<typename T>
inline T&
Resource_handler<T>::rc() { return rc_; }

template<typename T>
inline const T& 
Resource_handler<T>::rc() const { return rc_; }

// -------------------------------------------------------------------------- //
// Registry

inline
Handler_registry::Handler_registry() 
  : std::vector<Handler*>(FD_SETSIZE, nullptr), active_(0), max_(-1)
{ }

inline const ff::Select_set&
Handler_registry::wait() const { return wait_; }

inline int
Handler_registry::max() const { return max_; }

/// Insert a new handler into the registry. The handler is directly mapped
/// by its file descriptor. Behavior is undefined a handler is already
/// registerd for that file descriptor. 
///
/// The handler's open() method is called when the service is added. If
/// open() returns false, the handler is not added, and the function
/// returns false.
inline bool
Handler_registry::add(Reactor& r, Handler* h) {
  assert(0 <= h->fd() and h->fd() < FD_SETSIZE);
  assert((*this)[h->fd()] == nullptr);
  
  // Initially insert the handler.
  (*this)[h->fd()] = h;
  ++active_;
  int m = std::max(h->fd(), max_);

  // Insert the reader into the appropriate set.
  // TODO: An event mask should specify the event sets
  // that we're actually interested in. 
  wait_.read.insert(h->fd());

  // Try to open the handler. Changes are committed if this
  // succeeds and rolled back if it fails.
  if (not h->on_open(r)) {
    (*this)[h->fd()] = nullptr;
    --active_;
    wait_.read.remove(h->fd());
    return false;
  } else {
    max_ = m;
    return true;
  }
}

/// Remove the service hander. The handler's close() method is called
/// when the handler is removed.
///
/// This function always returns true.
///
/// \todo: If active falls below some some low water mark, re-scan the
/// list for a new max value. It's not strictly necessary, since *nixes
/// recycle fds, but if active is very low and max is very high, then
/// we're going to have a very sparse list.
inline bool 
Handler_registry::remove(Reactor& r, Handler* h) {
  assert(0 <= h->fd() and h->fd() < FD_SETSIZE);
  (*this)[h->fd()] = nullptr; 
  --active_;
  wait_.read.remove(h->fd());
  h->on_close(r);
  return true;
}

} // namespace nocontrol
