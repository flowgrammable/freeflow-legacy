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

#include <iostream>
#include <string>
#include <map>
#include <utility>

#include <freeflow/sys/print.hpp>
#include <freeflow/sys/json.hpp>

namespace freeflow {

// Generic print function. Used to call specific print functions.
void
Printer::print(const json::Value& v) {
  switch(v.type()) {
  case json::Value::NIL: return print_null(v.as_null());
  case json::Value::BOOL: return print_bool(v.as_bool());
  case json::Value::INT: return print_int(v.as_int());
  case json::Value::REAL: return print_real(v.as_real());
  case json::Value::STRING: return print_string(v.as_string());
  case json::Value::ARRAY:  return print_array(v.as_array());
  case json::Value::OBJECT: return print_object(v.as_object());
  case json::Value::ERROR: return print_error();
  break;
    assert(false);
  }
}

void 
Printer::print_null(json::Null n) {
  os_ << n;
}

void
Printer::print_bool(json::Bool b) {
  os_ << b;
}

void 
Printer::print_int(json::Int z) {
  os_ << z;
}

void
Printer::print_real(json::Real r) {
  os_ << r;
}

void
Printer::print_string(json::String s) {
  os_ << s;
}

void 
Printer::print_pair(const json::Pair& p){
  os_ << p.first;
  print(p.second);
}

void 
Printer::print_pair(const json::String& s, const json::Value& v) {
  os_ << s;
  print(v);
}

// Print beginning of object
void Printer::start_object() {
  os_ << "{\n";
 ++indent_;
}

// Print end of object. 
void 
Printer::end_object() {
 --indent_;
 os_ << '\n';
 print_indent();
 os_ << "}\n";
}

// Print object
void 
Printer::print_object(const json::Object& o) {
  start_object();
  auto iter = o.begin();
  auto end = o.end();
  while (iter != end) {
    print_indent();
    print_pair(*iter);
    if (std::next(iter) != end)
      os_ << ",\n";
  }
  end_object();
}

// Print beginning of an array.
void 
Printer::start_array() {
  os_ << "[ \n";
  ++indent_;
}

// Print end of array
void 
Printer::end_array() {
  --indent_;
  os_ << '\n';
  print_indent();
  os_ << "] \n";
}

// Print arrays.
void 
Printer::print_array(const json::Array& a) {
  start_array();

  auto iter = a.begin();
  auto end = a.end();
  while (iter != end) {
    print(*iter);
    print_indent();
  }
  end_array();
}

// Print error messages
// TODO: Implement me!
void
Printer::print_error() { 
}

void
Printer::print_indent() {
  os_ << std::string(indent_ * 2, ' ');
}

} // freeflow

