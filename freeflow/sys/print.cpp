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
  switch(v.type_) {
    case json::Value::NIL: 
      print_null(v.data_.n);
    case json::Value::BOOL:
      print_bool(v.data_.b);
    case json::Value::INT:
      print_int(v.data_.z);
    case json::Value::REAL:
      print_real(v.data_.r);
    case json::Value::STRING:
      print_string(v.data_.s);
    case json::Value::ARRAY:
      print_array(v.data_.a);
    case json::Value::OBJECT:
      print_object(v.data_.o);
    case json::Value::ERROR:
      print_error();
    break;
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
  os_ << "{ \n";
 ++indent_; 
 tab_.insert(0, indent_, char(32));
 os_ << tab_ ;
}

// Print end of object. 
void 
Printer::end_object() {
 --indent_;
 tab_.clear();
 tab_.insert(0, indent_, char(32));
 os_ << "\n }," << tab_;
}

// Print object
void 
Printer::print_object(const json::Object& o) {
  start_object();
  for(auto& iter : o) {
    os_ << "\"" << iter.first << "\" :";
    print(iter.second);
  }
  end_object();
}

// Print beginning of an array.
void 
Printer::start_array() {
  os_ << "[ \n";
}

// Print end of array
void 
Printer::end_array() {
  os_ << "] \n";
}

// Print array
void 
Printer::print_array(const json::Array& a) {
  start_array();
  for(unsigned int x = 0; x < a.size(); x++) {
    print(a[x]);
  }
  end_array();
}

// Print error messages
void
Printer::print_error(){
  
}
} // freeflow

