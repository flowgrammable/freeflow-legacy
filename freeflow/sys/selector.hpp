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

#include <set>

#include "freeflow/sys/error.hpp"
#include "freeflow/sys/time.hpp"

namespace freeflow {

/// The descriptor set class is a helper class for the Selector. It 
/// maintains a set of file descriptors and caches an underlying bitfield
/// of that set. Note that the bitfield is only valid after calling
/// update() method.
struct Descriptor_set {
  Descriptor_set() 
    : max(-1)
  { FD_ZERO(&fds); }

  bool test(int fd) const { return FD_ISSET(fd, &fds); }

  void insert(int fd) { 
    FD_SET(fd, &fds);
    if (fd > max)
      max = fd;
  }
  
  // NOTE: Removing a file descriptor does not search for a new maximum 
  // file descriptor. We probably want a special functionality to
  // occasionally probe the fd set for a new max value. I don't think
  // we need to do this very often.
  void remove(int fd) { 
    FD_CLR(fd, &fds);
  }

  fd_set fds;
  int max;
};

/// The selector class provides a simple wrapper over the POSIX pselect
/// function. It maintains lists of resources being selected for reading
/// and writing.
class Selector {
public:
  // Resource management
  void add_reader(int fd);
  void add_writer(int fd);
  void remove_reader(int fd);
  void remove_writer(int fd);

  // Resource queries
  bool is_readable(int fd) const;
  bool is_writable(int fd) const;

  // Select
  int operator()();
  int operator()(Microseconds);

private:
  Descriptor_set readers;
  Descriptor_set writers;
};

} // namespace freeflow

#include "selector.ipp"

#endif
