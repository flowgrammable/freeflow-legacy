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

namespace freeflow {

// Pretty print json document

class Print {
  static int indent_count = 0;
  static std::string leading_spaces = "";

// Print object
  template<typename C, typename T, typename X>
  std::basic_ostream<C, T>& 
  pprint_object(std::basic_ostream& os, const X& a) {
    /*for (auto it = std::begin(os); it != std::end(os); ++it){

    }*/

  }

// Print array
  template<typname C, typename T, typename X>
  std::basic_ostream<C, T>&
  pprint_array(std::basic_ostream& os, const X& a) {
    
  }

 };




} // namespace

#endif
