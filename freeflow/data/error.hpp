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

#include <cstdint>

namespace freeflow {

/// There are two important classes of errors: data overflow and underflow.
/// An overflow occurs when the reading or writing of data through a View
/// would exceed the Views available bytes (cross a memory boundary). An
/// underflow occurs when completing a read or write does not reach the
/// boundary and would leave uninterpreted data in the buffer. Note that
/// underflow is sometimes a recoverable error.
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
struct Error {
  /// The error code is an integer value indicating a specific kind of error.
  using Code = int;
  static constexpr Code SUCCESS = 0;            // Not an error
  static constexpr Code FAILURE = -1;           // Unspecified failure
  static constexpr Code BUFFER_OVERFLOW = 1;    // Insufficient bytes in buffer
  static constexpr Code STRING_OVERFLOW = 2;    // Insufficient bytes in string
  static constexpr Code SEQUENCE_UNDERFLOW = 3; // Excess data in sequence

  /// Associated data is interpreted by the error code. Errors involving 
  /// insufficient or excess data will associate the number of bytes by
  /// which a read or write would overflow or underflow a boundary.
  using Data = std::intptr_t;

  constexpr Error();
  constexpr Error(Code c, Data d = 0);

  constexpr operator bool() const;

  Code code;
  Data data;
};

/// Like an error object, but opposite.
///
///    if (Failure f = valid(x))
///     cout << f.code();
struct Failure {
  constexpr Failure(Error e)
    : err(e) { }

  constexpr operator bool() const { return !err; }

  Error err;
};


constexpr Error ok(bool b, Error err);


} // namespace freeflow

#include "error.ipp"

#endif
