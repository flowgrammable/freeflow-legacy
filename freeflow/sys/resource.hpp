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

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <unistd.h>
#include <fcntl.h>

#include <cassert>
#include <cstddef>

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>

namespace freeflow {

class Buffer;

/// The Resource class is the base class of all POSIX resources that
/// are interanally represented by a file descriptor.
class Resource {
public:
  Resource();

  // Noncopyable.
  Resource(const Resource&) = delete;
  Resource& operator=(const Resource&) = delete;

  // Move semantics.
  Resource(Resource&& r);
  Resource& operator=(Resource&& r);

  // Resource initialization
  explicit Resource(int f);

  ~Resource();

  /// Evaluates to true only when the resource is valid.
  explicit operator bool() const;

  /// Returns the file descriptor.
  int fd() const;

  // Read
  std::size_t read(void*, std::size_t);
  std::size_t read(Buffer&, std::size_t);
  std::size_t read(Buffer&);
  
  // Write
  std::size_t write(const void*, std::size_t);
  std::size_t write(const Buffer&, std::size_t);
  std::size_t write(const Buffer&);

  // Status flags
  int get_status() const;
  void set_status(int);
  
  // Blocking status
  bool is_blocking() const;
  void set_blocking();
  void set_nonblocking();

private:
  int fd_;
};

// Operations
std::size_t read(Resource&, void*, std::size_t);
std::size_t read(Resource&, Buffer&, std::size_t);
std::size_t read(Resource&, Buffer&);

std::size_t write(Resource&, const void*, std::size_t);
std::size_t write(Resource&, const Buffer&, std::size_t);
std::size_t write(Resource&, const Buffer&);

/// The Resource_set class represents the inclusion of resources in
/// a particular set.
///
/// While this is primarily used by the Selector and related reactor
/// loops, it is still a reasonably important concept to include
/// in a separate module.
struct Resource_set {
  explicit Resource_set();

  bool test(const Resource&) const;
  bool test(int) const;

  void insert(const Resource&);
  void insert(int);
  
  void remove(const Resource&);
  void remove(int);
  void clear();

  fd_set fds;
};

} // namespace resource

#include <freeflow/sys/resource.ipp>

#endif
