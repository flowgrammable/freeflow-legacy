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
bool
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
bool 
Handler_registry::remove(Reactor& r, Handler* h) {
  assert(0 <= h->fd() and h->fd() < FD_SETSIZE);
  (*this)[h->fd()] = nullptr; 
  --active_;
  wait_.read.remove(h->fd());
  h->on_close(r);
  return true;
}

} // namespace freeflow
