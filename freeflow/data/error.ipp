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

/// Initialize the message with the SUCCESS code.
constexpr 
Error::Error() : code(SUCCESS), data() { }

/// Initialize the message result with the result code and data.
constexpr 
Error::Error(Code c, Data d) : code(c), data(d) { }

/// Allows contextual conversion to bool, returning true iff and only
/// if code != SUCCESS 
constexpr 
Error::operator bool() const { return code == SUCCESS; }

/// Returns an error condition based on a predicate.
///
/// If the condition b is true, the resulting error condition will evaluate
/// to SUCCESS. Otherwise, the condition will evaluate to the code c.
constexpr Error 
ok(bool b, Error err) { return b ? Error() : err; }

} // namespace freeflow
