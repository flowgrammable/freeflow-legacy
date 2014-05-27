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

#ifndef FREEFLOW_PARAMETER_HPP
#define FREEFLOW_PARAMETER_HPP

#include <functional>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <map>

//#include <command.hpp>
#include <freeflow/sys/json.hpp>

namespace ff = freeflow;

namespace cli {
  
using Type = std::function<freeflow::json::Value(std::string)>;

class Parameter {
public:
  /// The Name struct contains the name and alias of a parameter
  struct Name {
    std::string name;
    std::string alias;
  };

  /// The Initializer struct contains an enumeration that tells the constructor
  /// how the parameter should be constructed and a string containing the 
  /// default value for the parameter. The default value is only used when the
  /// state is PRESENT
  struct Initializer {
    enum state { OPTIONAL, REQUIRED, PRESENT };
    std::string value;
  };

  //Constructors

  //Accessors
  Name name();
  std::string doc();
  Type& type();

private:
  Name name_;
  Initializer init_;
  std::string doc_;
  Type& type_;
};

struct Bool {
  ff::json::Value operator()(const std::string&) const;
};

struct Real {
  ff::json::Value operator()(const std::string&) const;
};

template<typename T>
struct Optional {
  ff::json::Value operator()(const std::string&) const;
};

} // namespace cli

#endif
