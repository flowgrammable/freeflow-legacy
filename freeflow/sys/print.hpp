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

#ifndef FREEFLOW_PRINT_HPP
#define FREEFLOW_PRINT_HPP

#include <iosfwd>
#include <string>

#include <freeflow/sys/json.hpp>

namespace freeflow {

/// The Printer class provides facilities to suppor the pretty-printing
/// of values in freeflow and its applications. The pretty printer is
/// designed to print JSON-structured documents. The "terminal" printed
/// elements are the set of value types supported by the JSON library.
/// Support is also provided for objects and array.
///
/// To make the Printer adaptable to non-json data types, operations
/// are provided to crate object and array enclosures.
///
/// \todo Should this be put into the JSON namespace and then aliased
/// into freeflow?
class Printer {
public:
  Printer(std::ostream& os)
    : os_(os), indent_(0) { }

  void print(const json::Value&);
  void print_null(json::Null);
  void print_bool(json::Bool);
  void print_int(json::Int);
  void print_real(json::Real);
  void print_pair(const json::Pair&);
  void print_pair(const json::String&, const json::Value&);
  void print_object(const json::Object&);
  void print_array(const json::Array&);
  //void print_error(const json::Error&);

  // Object formatting
  void start_object();
  void end_object();

  // Array formatting
  void start_array();
  void end_array();

  std::ostream& os_;     // The underlying stream     
  int           indent_; // The current indent level
  std::string   tab_;    // A string contaning the current "tab".
 };

} // namespace

#include "print.ipp"

#endif
