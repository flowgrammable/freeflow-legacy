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

/// Allow implicit upcasting from derived types.
template<typename T, typename X>
  constexpr
  Error::Error(T err)
    : code_(err.code()), data_(err.data()) { }

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

/// Returns true when two errors have the same error code.
inline bool 
operator==(Error a, Error b) { return a.code() == b.code(); }

inline bool 
operator!=(Error a, Error b) { return not(a == b); }


// -------------------------------------------------------------------------- //
// System error

/// Construct a system error from the given result code. This associates
/// the POSIX error code as the result type.
inline
System_error::System_error(int r) 
  : Error(r < 0 ? SYSTEM_ERROR : SUCCESS, errno) { }


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

/// Returns the underlying error.
inline Error
Trap::error() const { return err_; }

/// Returns the error code.
inline Error::Code 
Trap::code() const { return err_.code(); }

/// Returns the error data.
inline Error::Data
Trap::data() const { return err_.data(); }


} // namespace freeflow
