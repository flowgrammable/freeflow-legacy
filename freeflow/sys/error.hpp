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
#include <system_error>
#include <stdexcept>

#include <freeflow/sys/meta.hpp>

namespace freeflow {

/// The Error_category type provdes a classifier for error codes and
/// provides facilities for generating error messgaes.
///
/// \todo Make this an actual class and provide an overload of message()
/// that takes additional error data for custom formatting.
using Error_category = std::error_category;

using Error_code = std::error_code;

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
  /// The error code is an internal representation of the error. This
  /// combines a numeric code with an error category.
  using Code = Error_code;

  /// Associated data is interpreted by the error code. Errors involving 
  /// insufficient or excess data will associate the number of bytes by
  /// which a read or write would overflow or underflow a boundary.
  using Data = std::intptr_t;

  Error();
  Error(Code, Data = 0);

  explicit operator bool() const;

  const Error_category& category() const;
  Code code() const;
  Data data() const;
  std::string message() const;

private:
  Code code_;
  Data data_;
};

// Equality comparison
bool operator==(Error, Error);
bool operator!=(Error, Error);


// Error constructors.
Error ok(bool b, Error err);

Error system_error();
Error system_error(int n);


/// A Trap object is used to capture failures in if statements, allowing
/// the following kinds of programs.
///
///    if (Trap f = valid(x))
///     print(f.error());
class Trap {
public:
  Trap(Error e);

  explicit operator bool() const;
  operator Error() const;

  Error error() const;
  Error::Code code() const; 
  Error::Data data() const;


private:
  Error err_;
};

} // namespace freeflow

#include "error.ipp"

#endif
