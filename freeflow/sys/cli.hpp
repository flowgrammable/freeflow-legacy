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
#include <list>
#include <sstream>
#include <unordered_map>
#include <iostream>

#include <freeflow/sys/json.hpp>

namespace freeflow {
namespace cli {

// -------------------------------------------------------------------------- //
// Value types

// An extension of the JSON model that indicates an error.
struct Error { };

/// The Value class extends JSON values to include additional error
/// information.
class Value : public json::Value {
public:
  static constexpr Error error { };

  using json::Value::Value;

  Value() = default;
  Value(Error);

  explicit operator bool() const;
private:
  bool error_ = false;
};


// -------------------------------------------------------------------------- //
// Parameters

/// The type of a parameter is determined by a function. If a string can be
/// converted into a JSON value, then the type is assumed to have checked.
using Type = std::function<Value(const std::string&)>;


/// The Name struct contains the name and alias of a parameter.
struct Name {
  Name(const char* s);
  Name(const std::string& s);

  std::string name;
  std::string alias;
};


/// The Valuation semantics describe properties of a parameters value:
/// whether or not it is required, or a default has been provided.
enum Valuation {
  /// An optional parameter is one that need not be specified in the
  /// command line arguments.
  OPTIONAL, 

  /// A required parameter must be specified in the command line
  /// arguments.
  REQUIRED, 

  /// A parameter with a default argument is initialized to that value
  /// only if no command line argument is given.
  DEFAULT
};

/// Represents properties of the parameters valuation. Note that
/// the value is present only if which is DEFAULT.
///
/// \todo Write constructors.
struct Initializer {
  Initializer();
  Initializer(Valuation v);
  Initializer(const char* s);
  Initializer(const std::string& s);

  Valuation which;
  std::string value;
};


/// The Parameter embodies the declaration of a command line parameter.
/// This includes its name (possibly including an alias or short name),
/// its type (given as a validating function), a value property that
/// provides additional semantics for the parameter, and its documentation.
class Parameter {
public:

  //Constructors
  Parameter(const std::string&, const Type&, const Initializer&, const std::string&);

  //Accessors
  bool has_alias() const;

  const std::string& name() const;
  const std::string& alias() const;
  const Type& type() const;
  const Initializer& init() const;
  const std::string& doc() const;

private:
  Name        name_;
  Type        type_;
  Initializer init_;
  std::string doc_;
};


/// The Parameter_set contains a set of parameters, which are declared
/// by the user, and used by the compiler to parse command line arugments.
class Parameters {
  using Parm_list = std::list<Parameter>;
  using Parm_map = std::unordered_map<std::string, Parameter*>;
public:

  // Parameter declaration
  Parameter* declare(const std::string&, 
                     const Type&, 
                     const Initializer&, 
                     const std::string&);

//private:
  Parm_list parms_;
  Parm_map map_;
};


// Stores name/value and positional arguments. The associated value type
// is given as the template parameter T.
template<typename T>
  struct Argument_store {
    using Argument_value = T;
    using Argument_map = std::map<std::string, Argument_value>;
    using Argument_list = std::vector<Argument_value>;

    Argument_map  named;  // Name/value mappings
    Argument_list listed; // Positional arguments.
  };

/// The Arguments type contains the parsed command line options, binding
/// parameter names to values, and also the positional arguments.
using Arguments = Argument_store<Value>;


// -------------------------------------------------------------------------- //
// Type checking

// Type checkers
struct Null { Value operator()(const std::string&) const; };
struct Bool { Value operator()(const std::string&) const; };
struct Int { Value operator()(const std::string&) const; };
struct Real { Value operator()(const std::string&) const; };
struct String { Value operator()(const std::string&) const; };

// Type checking combinators
template<typename T>
  struct Optional { Value operator()(const std::string&) const; };

template<typename T>
  struct Sequence { Value operator()(const std::string&) const; };


// -------------------------------------------------------------------------- //
// Parsing

/// A support structure for parsing coniguration by parts.
using Parsed_arguments = Argument_store<json::Value>;

// Parsing functions
Arguments parse_args(const Parameters&, int, char*[]);

Arguments parse_env(const Parameters&, const char*);
Arguments parse_env(const Parameters&, const std::string&);

Arguments parse_config(const Parameters&, const char*);
Arguments parse_config(const Parameters&, const std::string&);



} // namespace cli
} // namespace freeflow

std::string toupper(const std::string&); // toupper for string
std::string toupper(const char*);        // toupper for char[]


#include <freeflow/sys/cli.ipp>

#endif
