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
Handler::Handler(Resource& r)
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

inline const Select_set&
Handler_registry::wait() const { return wait_; }

inline int
Handler_registry::max() const { return max_; }


} // namespace nocontrol
