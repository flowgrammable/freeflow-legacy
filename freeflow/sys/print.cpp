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
 ++indent_; 
 tab_.insert(0, indent_, char(32));
 os_ << "\n" << tab_ ;
}

// Print end of object. 
void 
Printer::end_object() {
 --indent_;
 tab_.clear();
 tab_.insert(0, indent_, char(32));
 os_ << "\n" << tab_;
}

// Print object
void 
Printer::print_object(const json::Object& o) {

}

// Print beginning of an array.
void 
Printer::start_array() {

}

// Print end of array
void 
Printer::end_array() {

}

// Print array
void 
Printer::print_array(const json::Array& a) {

}

// Print error messages
/*
void
Printer::print_error(const json::Error& e){
  os_ << e;
}
*/

} // freeflow

