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

#ifndef FREEFLOW_SELECTOR_HPP
#define FREEFLOW_SELECTOR_HPP

#include <unistd.h>
#include <cassert>

#include <set>

#include "freeflow/sys/error.hpp"
#include "freeflow/sys/time.hpp"
#include "freeflow/sys/resource.hpp"

namespace freeflow {

/// The descriptor set class is a helper class for the Selector. It 
/// maintains a set of file descriptors and caches an underlying bitfield
/// of that set. Note that the bitfield is only valid after calling
/// update() method.
struct Resource_set {
  explicit Resource_set() { FD_ZERO(&fds); }

  /// Returns tre
  bool test(const Resource& r) { return test(r.fd()); }
  
  bool test(int fd) const { return FD_ISSET(fd, &fds); }

  // Insert the reader into the set.
  void insert(const Resource& r) { insert(r.fd()); }

  // Insert the reader into the set.
  void insert(int fd) { 
    assert(fd != -1);
    FD_SET(fd, &fds);
  }
  
  void remove(int fd) { FD_CLR(fd, &fds); }

  // Remove all files from the set.
  void clear() { FD_ZERO(&fds); }

  fd_set fds;
};

/// The selector class provides a simple wrapper over the POSIX pselect
/// function. It maintains the sets of file descriptors used to indicate
/// the resources that are available for reading and writing.
class Selector {
public:
  Selector(int n, Resource_set*, Resource_set* = nullptr, Resource_set* = nullptr);

  // Select
  int operator()();

private:
  int     max_;
  fd_set* read_;
  fd_set* write_;
  fd_set* error_;
};

} // namespace freeflow

#include "selector.ipp"

#endif
