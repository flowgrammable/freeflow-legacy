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

#ifndef FREEFLOW_RESOURCE_HPP
#define FREEFLOW_RESOURCE_HPP

#include <unistd.h>
#include <fcntl.h>

#include <cassert>
#include <cstddef>

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>


namespace freeflow {

class Buffer;

/// A System result indicates the result status of an operation. An 
/// operation can complete in one of four states: the operation can fail,
/// it can succeed, the result can be deferred (will be available in the
/// future), or it can be interrupted (and should be retried).
///
/// Note
///
/// \todo This should really be a template, indicating the availability
/// of an arbitrary return type...
class System_result {
public:
  enum Status { FAILED, COMPLETED, DEFERRED, INTERRUPTED };
  using Value_type = std::size_t;
  using Error_type = Error;

  // Copy semantics
  System_result(const System_result&);
  System_result& operator=(const System_result&);

  explicit System_result(Status);
  explicit System_result(Status, Error_type);
  explicit System_result(Status, Value_type);
  System_result(ssize_t n);

  // Status tests
  bool failed() const;
  bool completed() const;
  bool deferred() const;
  bool interrupted() const;

  // Accessors
  Status status() const;
  Error_type error() const;
  Value_type value() const;

  // Tests
  bool has_value(Value_type) const;

  // Conversion
  explicit operator bool() const;
  operator Trap() const;

  // Creators
  // TODO: Write other creators.
  static System_result fail(int);
  static System_result defer();
  static System_result defer(int);
  static System_result complete(std::size_t n);

private:
  void copy_value(const System_result&);
  void init_value(ssize_t);

private:
  Status status_;  // The operation result status
  union Data {
    Data();
    Error_type error;  // An error code.
    Value_type value;  // An integer value.
  } data_;
};

// Equality
bool operator==(const System_result&, const System_result&);
bool operator!=(const System_result&, const System_result&);

// Ordering
bool operator<(const System_result&, const System_result&);
bool operator>(const System_result&, const System_result&);
bool operator<=(const System_result&, const System_result&);
bool operator>=(const System_result&, const System_result&);


/// The Resource class is the base class of all POSIX resources that
/// are internally represented by a file descriptor.
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
  System_result read(void*, std::size_t);
  System_result read(Buffer&, std::size_t);
  System_result read(Buffer&);
  
  // Write
  System_result write(const void*, std::size_t);
  System_result write(const Buffer&, std::size_t);
  System_result write(const Buffer&);

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
System_result read(Resource&, void*, std::size_t);
System_result read(Resource&, Buffer&, std::size_t);
System_result read(Resource&, Buffer&);

System_result write(Resource&, const void*, std::size_t);
System_result write(Resource&, const Buffer&, std::size_t);
System_result write(Resource&, const Buffer&);

// Flags
bool is_blocking(const Resource&);
void set_blocking(Resource&);
void set_nonblocking(Resource&);


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
