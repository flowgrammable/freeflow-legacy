// Copyright (c) 2013-2014 Flowgrammable, LLC.
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

#ifndef FREEFLOW_ERROR_HPP
#define FREEFLOW_ERROR_HPP

#include <cassert>
#include <cerrno>
#include <cstdint>
#include <stdexcept>

namespace freeflow {

/// The Error class contains error codes and associated data from various
/// operations.
///
/// Usage in if statements:
///
////  Error valid(X x);
///
///   if (valid(x))
///     // x is valid
///
/// In other words the assignment as an error succeeds only when an error
/// is actually present. To capture the opposite, see the Valid 
class Error {
public:
  /// The error code is an unsigned value indicating a specific kind of 
  /// error.
  using Code = std::size_t;

  /// An error did not occur.
  static constexpr Code SUCCESS = 0;
  
  /// An unspecified failure. There is no associated error data.
  static constexpr Code FAILURE = -1;
  
  /// A POSIX system error. The error data is set to the corresponding
  /// error number (errno).
  static constexpr Code SYSTEM_ERROR = 1;
  
  /// An attempt to read or write past the end of a View or Buffer. The
  /// error data is the number of bytes by which the boundary was overrun.
  static constexpr Code BUFFER_OVERRUN = 2;
  
  /// Associated data is interpreted by the error code. Errors involving 
  /// insufficient or excess data will associate the number of bytes by
  /// which a read or write would overflow or underflow a boundary.
  using Data = std::intptr_t;

  constexpr Error();
  constexpr Error(Code c, Data d = 0);

  constexpr operator bool() const;

  Code code() const;
  Data data() const;

private:
  Code code_;
  Data data_;
};

/// A system error is an error code that represents a POSIX system error.
/// This class derives from Error, providing special constructors.
struct System_error : Error {
  System_error() = default;
  System_error(int);
};


// Error constructors.
Error ok(bool b, Error err);
Error system_error();


/// A Trap object is used to capture failures in if statements, allowing
/// the following kinds of programs.
///
///    if (Trap f = valid(x))
///     print(f.error());
class Trap {
public:
  constexpr Trap(Error e);

  constexpr operator bool() const;

  Error error() const;
  Error::Code code() const; 
  Error::Data data() const;

private:
  Error err_;
};

} // namespace freeflow

#include "error.ipp"

#endif
