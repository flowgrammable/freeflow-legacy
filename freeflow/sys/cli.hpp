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
#include <set>
#include <iostream>
#include <cstring>

#include <freeflow/sys/json.hpp>

namespace freeflow {
namespace cli {

class Command;



// -------------------------------------------------------------------------- //
// Parameters

/// The type of a parameter is determined by a function. If a string can be
/// converted into a JSON value, then the type is assumed to have checked.
using Type = std::function<json::Value(const json::Value&)>;


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
  json::Value value;
};

/// The Parameter embodies the declaration of a command line parameter.
/// This includes its name (possibly including an alias or short name),
/// its type (given as a validating function), a value property that
/// provides additional semantics for the parameter, and its documentation.
class Parameter {
public:

  // Constructors
  Parameter(const std::string&, const Type&, const Initializer&, const std::string&);

  // Accessors
  bool has_alias() const;

  const std::string& name() const;
  const std::string& alias() const;
  const Type& type() const;
  
  const Initializer& init() const;
  bool is_required() const;
  bool is_optional() const;
  bool has_default() const;
  const json::Value& default_argument() const;
  
  const std::string& doc() const;

private:
  Name        name_;
  Type        type_;
  Initializer init_;
  std::string doc_;
};


/// The Parameter_set contains a set of parameters, which are declared
/// by the user, and used by the compiler to parse command line arugments.
class Parameters : private std::list<Parameter> {
  using Base = std::list<Parameter>;
  using Parm_map = std::unordered_map<std::string, Parameter*>;
public:

  // Parameter declaration
  Parameter* declare(const std::string&, 
                     const Type&, 
                     const Initializer&, 
                     const std::string&);

  // Imported base operations
  using Base::begin;
  using Base::end;

//private:
  Parm_map  map_;
};


// -------------------------------------------------------------------------- //
// Location

enum Source {
  ENVIRONMENT,
  CONFIG,
  COMMAND_LINE,
  FROM_DEFAULT,
  NOT_PROVIDED
};

class Location {
public:
  Location() = default;
  Location(const Source&);

  void set_source(const Source&);
  Source get_source() const;

private:
  Source source_;
  // TODO: Add json line number/location/index info
};


// -------------------------------------------------------------------------- //
// Arguments
class Argument {
public:
  Argument() = default;
  Argument(const json::Value&, const Source&);

  // Mutators
  void set_value(const json::Value&);
  void set_source(const Source&);

  // Accessors
  json::Value get_value() const;
  Source get_source() const;

private:
  json::Value value_;
  Location loc_;
};

/// The Arguments type contains the parsed command line options, binding
/// parameter names to values, and also the positional arguments.
class Arguments {
public:
  using Argument_map  = std::map<std::string, Argument>;
  using Argument_list = std::vector<json::Value>;

  // Keyword arguments
  bool has_named(const Parameter&);
  bool has_named(const std::string&);
  void set_named(const Parameter&, const Argument&);
  void set_named(const std::string&, const Argument&);
  json::Value get_named_value(const std::string&) const;
  Argument get_named_arg(const std::string&) const;
  
  
  // Positional arguments
  void set_listed(const json::Value&);
  json::Value get_listed(const int&) const;
  int get_listed_size() const;

  // Error handling
  void display_errors(const Command&, const char*);

private:
  Argument_map     named_;    // Name/value mappings
  Argument_list    listed_;   // Positional arguments.
};


// -------------------------------------------------------------------------- //
// Commands

class Command;
class Commands;

/// The command class represents a command-line command ...
class Command : private Parameters {
public:
  virtual ~Command();
  Command(const std::string&, const std::string&);

  virtual bool run(const Arguments&) = 0;

  // Accessors
  const Parameters& parms() const;
  const std::string& name() const;
  const std::string& help() const;

  // Imported base class operations
  using Parameters::declare;

  std::string name_;
  std::string doc_;
};

/// The Help_command is a built-in command that is used to print
/// help text for a program and its commands.
class Help_command : public Command {
public:
  Help_command(Commands&);

  bool run(const Arguments&) override;

private:
  void help();
  void help(const Command&);

private:
  Commands& cmds_;
};


/// The commands class manages the map of command names to commands ...
///
/// \todo Is this class movable? Is it copyable?
class Commands : private std::map<std::string, Command*> {
  using Base = std::map<std::string, Command*>;
public:
  Commands();
  ~Commands();

  // Command declaration
  template<typename T, typename... Args>
    Command* declare(Args&&...);

  // Base class imports
  using Base::operator[];
  using Base::count;
  using Base::find;
  using Base::begin;
  using Base::end;
};

// -------------------------------------------------------------------------- //
// Type checking

// Type checkers
struct Null_typed { json::Value operator()(const json::Value&) const; };
struct Bool_typed { json::Value operator()(const json::Value&) const; };
struct Int_typed { json::Value operator()(const json::Value&) const; };
struct Real_typed { json::Value operator()(const json::Value&) const; };
struct String_typed { json::Value operator()(const json::Value&) const; };

// Type checking combinators
template<typename T>
  struct Optional_typed { json::Value operator()(const json::Value&) const; };

template<typename T>
  struct Sequence_typed { json::Value operator()(const json::Value&) const; };


// -------------------------------------------------------------------------- //
// Parsing

struct Parse_state {
  Parse_state() = default;
  Parse_state(const int&, const int&, char*[]);

  int argc;
  int current;
  char** argv;
};

// Parsing functions

void parse_keyword_args(const Parameters&, Arguments&, Parse_state&);
void parse_args(const Parameters&, Arguments&, Parse_state&);

void parse_env(const Parameters&, Arguments&, const char*);
void parse_env(const Parameters&, Arguments&, const std::string&);

void parse_config(const Parameters&, Arguments&, const char*);
void parse_config(const Parameters&, Arguments&, const std::string&);

bool check_type(const Parameter&, Arguments&, const json::Value&);

bool check_args(const Parameters&, Arguments&);

bool parse(const Parameters&, 
           const Commands&, 
           Arguments&, 
           int, 
           char*[], 
           const char*);

inline std::string display_err_info(const json::Value&/*json::Error&*/);

} // namespace cli
} // namespace freeflow

std::string toupper(const std::string&); // toupper for string
std::string toupper(const char*);        // toupper for char[]


#include <freeflow/sys/cli.ipp>

#endif
