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

namespace freeflow {

// -------------------------------------------------------------------------- //
// System result

namespace resource_impl {
// Translate the POSIX result into a system result type.
inline System_result::Status
type(ssize_t n) {
  if (n < 0) {
    if (errno == EINPROGRESS)
      return System_result::DEFERRED;
    else if (errno == EINTR)
      return System_result::INTERRUPTED;
    else
      return System_result::FAILED;
  }
  return System_result::COMPLETED;
}
} // naemspace resource_impl

inline
System_result::Data::Data() : error() { }

inline
System_result::System_result(const System_result& x)
  : status_(x.status_)
{
  copy_value(x);
}

inline System_result&
System_result::operator=(const System_result& x) {
  status_ = x.status_;
  copy_value(x);
  return *this;
}

/// Used to put 
inline
System_result::System_result(Status s)
  : status_(s), data_() { }

inline
System_result::System_result(Status s, Error_type e)
  : status_(s) { data_.error = e; }

inline
System_result::System_result(Status s, Value_type x)
  : status_(s) { data_.value = x; }

inline
System_result::System_result(ssize_t n)
  : status_(resource_impl::type(n))
{
  init_value(n);
}

/// Returns true if the operation failed. Reading any outputs, including
/// the value of this result, will result in undefined behavior.
inline bool
System_result::failed() const { return status_ == FAILED; }

/// Returns true if the operation completed, and the value all outputs
/// are available.
inline bool
System_result::completed() const { return status_ == COMPLETED; }

/// Returns true if the completion of the operation is deferred until
/// later. Reading any outputs of the operation will result in undefined
/// behavior, except that they may be reused as arguments to the operation.
inline bool
System_result::deferred() const { return status_ == DEFERRED; }

/// Returns true if the completion of the operation is deferred until
/// later. Reading any outputs of the operation will result in undefined
/// behavior, except that they may be reused as arguments to the operation.
///
/// Note that interrupted operations are expected to be retried.
inline bool
System_result::interrupted() const { return status_ == INTERRUPTED; }

inline System_result::Status
System_result::status() const { return status_; }

/// Returns the available value of the operation. Behavior is undefined
/// if the result is an error or is unavailable.
inline System_result::Value_type
System_result::value() const { 
  assert(completed());
  return data_.value; 
}

/// Returns the error value of the result. If the operation was completed,
/// deferred, or interrupted, this returns success.
inline System_result::Error_type
System_result::error() const {
  if (failed())
    return data_.error;
  else
    return Error();
}

/// Returns true if the operation completed with a value equal to
/// that given.
inline bool
System_result::has_value(Value_type x) const {
  return completed() and data_.value == x;
}

/// Returns true if the operation completed.
inline
System_result::operator bool() const { return completed(); }

/// Returns a trapped error code.
inline
System_result::operator Trap() const { return error(); }

/// Create a failed result with the given error code.
inline System_result
System_result::fail(int n) { return System_result(FAILED, system_error(n)); }

/// Create a deferred result.
inline System_result
System_result::defer() { return System_result(DEFERRED); }

/// Create a deferred result with the given error code.
inline System_result
System_result::defer(int n) { return System_result(DEFERRED, system_error(n)); }

/// Create a completed result with the given value.
inline System_result
System_result::complete(std::size_t n) { return System_result(COMPLETED, n); }

inline void
System_result::copy_value(const System_result& x) {
  if (failed())
    data_.error = x.data_.error;
  else if (completed())
    data_.value = x.data_.value;
}

inline void
System_result::init_value(ssize_t n) { 
  if (failed())
    data_.error = system_error(errno);
  else if (completed())
    data_.value = n;
}

/// Returns true if two results have the same value.
inline bool
operator==(const System_result& a, const System_result& b) {
  return a.value() == b.value();
}

inline bool
operator!=(const System_result& a, const System_result& b) {
  return a.value() != b.value();
}

/// Returns true if the result a compares less than the result b.
inline bool
operator<(const System_result& a, const System_result& b) {
  return a.value() < b.value();
}

inline bool
operator>(const System_result& a, const System_result& b) {
  return a.value() > b.value();
}

inline bool
operator<=(const System_result& a, const System_result& b) {
  return a.value() <= b.value();
}

inline bool
operator>=(const System_result& a, const System_result& b) {
  return a.value() >= b.value();
}

// -------------------------------------------------------------------------- //
// Resource


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

/// Read n bytes into the buffer. Behavior is undefined if n is greater than
/// or equal to the number of bytes allocated to buf.
///
/// The operation will be retried if interrupted by a signal.
inline System_result
Resource::read(void* buf, std::size_t n) { 
  while (true) {
    System_result r = ::read(fd(), buf, n); 
    if (r.interrupted())
      continue;
    return r;
  }
}

/// Read n bytes into the given buffer.
inline System_result
Resource::read(Buffer& buf, std::size_t n) {
  assert(n <= buf.size());
  return read(buf.data(), n);
}

/// Read data from the file into the buffer, returning the number of
/// bytes actually read.
inline System_result
Resource::read(Buffer& buf) { return read(buf.data(), buf.size()); }

/// Write n bytes from the given buffer. Behavior is undefined if n is
/// greater than or equal to the numbe of bytes allocated to buf.
///
/// The operation will be retried if interrupted by a signal.
inline System_result
Resource::write(const void* buf, std::size_t n) { 
  while (true) {
    System_result r = ::write(fd(), buf, n);
    if (r.interrupted())
      continue;
    return r;
  }
}

/// Write n bytes from the given buffer.
inline System_result
Resource::write(const Buffer& buf, const std::size_t n) {
  assert(n <= buf.size());
  return write(buf.data(), n);
}

/// Write data from the buffer to the file, returning the number of
/// bytes actually written.
inline System_result
Resource::write(const Buffer& buf) { return write(buf.data(), buf.size()); }

/// Return the status flags of the resource. 
inline int
Resource::get_status() const {
  System_result r = ::fcntl(fd(), F_GETFL, 0);
  if (r.failed())
    throw system_error();
  return r.value();  
}

/// Set the status flags for the resource.
inline void
Resource::set_status(int flags) {
  System_result r = ::fcntl(fd(), F_SETFL, flags);
  if (r.failed())
    throw system_error();
}

/// Returns true if certain resource operations block. Examples include
/// reads and writs on files, and accepts, conncets, receives, and sends
/// on sockets.
inline bool
Resource::is_blocking() const { return get_status() & O_NONBLOCK; }

/// Cause certain resource operations to block.
///
/// \see is_blocking
inline void
Resource::set_blocking() { set_status(get_status() ^ O_NONBLOCK); }

/// Cause certain resource operations to return immediately instead of
/// blocking. Potentially blocking operations return a system error of
/// EAGAIN or EWOULDBLOCK.
///
/// \see is_blocking.
inline void
Resource::set_nonblocking() { set_status(get_status() | O_NONBLOCK); }


// -------------------------------------------------------------------------- //
// Operations

inline System_result
read(Resource& rc, void* buf, std::size_t n) { return rc.read(buf, n); }

inline System_result
read(Resource& rc, Buffer& buf, std::size_t n) { return rc.read(buf, n); }

inline System_result
read(Resource& rc, Buffer& buf) { return rc.read(buf); }

inline System_result
write(Resource& rc, const void* buf, std::size_t n) { return rc.write(buf, n); }

inline System_result
write(Resource& rc, const Buffer& buf, std::size_t n) { return rc.write(buf, n); }

inline System_result
write(Resource& rc, const Buffer& buf) { return rc.write(buf); }

inline bool
is_blocking(const Resource& rc) { return rc.is_blocking(); }

inline void
set_blocking(Resource& rc) { return rc.set_blocking(); }

inline void
set_nonblocking(Resource& rc) { return rc.set_nonblocking(); }

// -------------------------------------------------------------------------- //
// Resource set

inline
Resource_set::Resource_set() { FD_ZERO(&fds); }

/// Returns true if the resource is in the set.
inline bool 
Resource_set::test(const Resource& r) const { return test(r.fd()); }

/// Returns true if the file descriptor is in the set.
inline bool 
Resource_set::test(int fd) const { 
  assert(fd != -1);
  return FD_ISSET(fd, &fds); 
}

/// Insert the resource into the set.
inline void 
Resource_set::insert(const Resource& r) { insert(r.fd()); }

/// Insert the file descriptor into the set.
inline void 
Resource_set::insert(int fd) { 
  assert(fd != -1);
  FD_SET(fd, &fds);
}

/// Remove the resource from the set.
inline void
Resource_set::remove(const Resource& r) { remove(r.fd()); }

/// Remove the file descriptor from the set.
inline void 
Resource_set::remove(int fd) { 
  assert(fd != -1);
  FD_CLR(fd, &fds); 
}

// Remove all files from the set.
inline void 
Resource_set::clear() { FD_ZERO(&fds); }

} // namespace freeflow
