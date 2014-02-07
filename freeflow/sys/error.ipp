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

namespace freeflow {

// -------------------------------------------------------------------------- //
// Error

/// Initialize the message with the SUCCESS code.
constexpr 
Error::Error() : code_(SUCCESS), data_() { }

/// Initialize the message result with the result code and data.
constexpr 
Error::Error(Code c, Data d) : code_(c), data_(d) { }

/// Allows contextual conversion to bool, returning true iff and only
/// if code != SUCCESS 
constexpr 
Error::operator bool() const { return code_ == SUCCESS; }

/// Returns the error code.
inline Error::Code 
Error::code() const { return code_; }

/// Returns the error data.
inline Error::Data
Error::data() const { return data_; }


// -------------------------------------------------------------------------- //
// Error constructors

/// Returns an error condition based on a predicate.
///
/// If the condition b is true, the resulting error condition will evaluate
/// to SUCCESS. Otherwise, the condition will evaluate to the code c.
inline Error 
ok(bool b, Error err) { return b ? Error() : err; }

/// Returns an error code that encapsulates the current system error.
inline Error
system_error() { return {Error::SYSTEM_ERROR, errno}; }


// -------------------------------------------------------------------------- //
// Trap

/// Initialzie the trap with an error. This allows implicit conversions
/// to trap objects.
constexpr 
Trap::Trap(Error e)
  : err_(e) { }

/// Allows contextual conversion to bool, returning true if and only if
/// the underlying error indicates failure.
constexpr 
Trap::operator bool() const { return !err_; }

/// Returns the error code.
inline Error::Code 
Trap::code() const { return err_.code(); }

/// Returns the error data.
inline Error::Data
Trap::data() const { return err_.data(); }


} // namespace freeflow
