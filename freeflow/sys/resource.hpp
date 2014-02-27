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

namespace freeflow {

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
  explicit operator bool() const { return fd_ >= 0; }

  /// Returns the file descriptor.
  int fd() const { return fd_; }

private:
  int fd_;
}; 

} // namespace resource

#include <freeflow/sys/resource.ipp>

#endif
