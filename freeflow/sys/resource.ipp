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

#include "resource.hpp"

namespace freeflow {

/// Initialize the resource in an invalid state.
inline
Resource::Resource()
  : fd_(-1) { }

/// Initialize the resource by moving it from r, making r invalid.
inline
Resource::Resource(Resource&& r)
    : fd_(r.fd_)
  { r.fd_ = -1; }

/// Assign the resource by moving it from r, making r invalid.
inline
Resource& Resource::operator=(Resource&& r) {
  fd_ = r.fd_;
  r.fd_ = -1;
  return *this;
}

/// Initialize the file resource with a file descriptor.
inline
Resource::Resource(int f)
  : fd_(f) { }

/// Close the resource.
inline
Resource::~Resource() { 
  if (*this)
    ::close(fd_); 
}

/// Returns true when the underlyng descriptor is valid.
inline
Resource::operator bool() const { return fd_ >= 0; }

/// Returns the file descriptor.
inline int
Resource::fd() const { return fd_; }

inline std::size_t
Resource::read(void* buf, std::size_t n) {
  ssize_t r = ::read(fd(), buf, n);
  if (r < 0)
    throw system_error();
  return r;
}

inline std::size_t
Resource::read(Buffer& buf, std::size_t n) {
  assert(n <= buf.size());
  return read(buf.data(), n);
}

/// Read data from the file into the buffer, returning the number of
/// bytes actually read.
inline std::size_t
Resource::read(Buffer& buf) {
  return read(buf.data(), buf.size());
}

inline std::size_t
Resource::write(const void* buf, std::size_t n) {
  ssize_t r = ::write(fd(), buf, n);
  if (r < 0)
    throw system_error();
  return r;
}

inline std::size_t
Resource::write(const Buffer& buf, const std::size_t n) {
  assert(n <= buf.size());
  return write(buf.data(), n);
}

/// Write data from the buffer to the file, returning the number of
/// bytes actually written.
inline std::size_t
Resource::write(const Buffer& buf) {
  return write(buf.data(), buf.size());
}

// -------------------------------------------------------------------------- //
// Operations

inline std::size_t
read(Resource& rc, void* buf, std::size_t n) { return rc.read(buf, n); }

inline std::size_t
read(Resource& rc, Buffer& buf, std::size_t n) { return rc.read(buf, n); }

inline std::size_t
read(Resource& rc, Buffer& buf) { return rc.read(buf); }

inline std::size_t
write(Resource& rc, const void* buf, std::size_t n) { return rc.write(buf, n); }

inline std::size_t
write(Resource& rc, const Buffer& buf, std::size_t n) { return rc.write(buf, n); }

inline std::size_t
write(Resource& rc, const Buffer& buf) { return rc.write(buf); }

} // namespace freeflow