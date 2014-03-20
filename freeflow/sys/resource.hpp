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


} // namespace resource

#include <freeflow/sys/resource.ipp>

#endif
