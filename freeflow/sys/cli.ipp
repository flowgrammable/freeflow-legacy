// Copyright (c) 2013-2014 Flowgrammable.org
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

#include <cctype>

namespace freeflow {
namespace cli {

namespace {

/// Transform a character array to be in all uppercase.
///
/// \todo Move these into a file alongside the String definition so
/// we don't have to keep re-defining them or making them members of
/// an anonymous namespace used in several translation units.
inline std::string
to_upper(const char* str) { 
  std::string upper = str;
  for (auto& c: upper) c = std::toupper(c); 
  return upper;
}

/// Transform a string to be in all uppercase
inline std::string
to_upper(const std::string& str) { return to_upper(str.c_str()); }

} // namespace

/// Return an environment variable name constructed from the given
/// prefix and parameter name.
inline std::string
make_env_var(const std::string& pre, const std::string& parm) {
  return to_upper(pre) + "_" + to_upper(parm);
}

// -------------------------------------------------------------------------- //
// Name

inline
Name::Name(const char* s) : name(""), alias(""){
  new (this) Name(std::string(s));
}

inline
Name::Name(const std::string& s) {
  std::size_t pos = 0;
  std::size_t n = s.find(", ", pos);
  if (n != s.npos) {
    name = s.substr(pos, n - pos);
    alias = s.substr(n + 2, s.npos - (n + 2));
  }
  else {
    name = s;
  }
}


// -------------------------------------------------------------------------- //
// Initializer

inline
Initializer::Initializer() 
  : which(OPTIONAL) { }

inline
Initializer::Initializer(Valuation v)
  : which(v) { }

inline
Initializer::Initializer(const char* s)
  : which(DEFAULT), value(s) { }

inline
Initializer::Initializer(const std::string& s)
  : which(DEFAULT), value(s) { }


// -------------------------------------------------------------------------- //
// Parameter

inline
Parameter::Parameter(const std::string& n, 
                     const Type& t, 
                     const Initializer& i, 
                     const std::string& d)
  : name_(n), type_(t), init_(i), doc_(d) { }

/// Returns true if the parameter has an alias. This is the case whenever
/// the name's alias field is non-empty.
inline bool
Parameter::has_alias() const { return not name_.alias.empty(); }

/// Returns the full name of the parameter. 
inline const std::string&
Parameter::name() const { return name_.name; }

/// Returns the alias of the parameter. If the string is empty, then
/// there is no alias.
inline const std::string&
Parameter::alias() const { return name_.alias; }

/// Returns the type of the parameter.
inline const Type& 
Parameter::type() const { return type_; }

/// Returns the parameter's initializer.
inline const Initializer&
Parameter::init() const { return init_; }


inline bool 
Parameter::is_required() const { 
  return init_.which == REQUIRED ? true : false; 
}

inline bool 
Parameter::is_optional() const { 
  return init_.which == OPTIONAL ? true : false; 
}

inline bool 
Parameter::has_default() const { 
  return init_.which == DEFAULT ? true : false; 
}

inline const json::Value& 
Parameter::default_argument() const {
  return init_.value;
}

/// Returns the string containing the documentation for the parameter. 
inline const std::string&
Parameter::doc() const { return doc_; }

inline Parameter* 
Parameters::declare(const std::string& n, 
                    const Type& t, 
                    const Initializer& i, 
                    const std::string& d)
{
  emplace_back(n, t, i, d);
  Parameter* p = &back();
  map_[p->name()] = p;
  if (p->has_alias())
    map_[p->alias()] = p;
  return p;
}


// -------------------------------------------------------------------------- //
// Command

inline
Command::Command(const std::string& n, const std::string& d)
  : name_(n), doc_(d) { }

inline
Command::~Command() { }

inline const Parameters&
Command::parms() const { return *this; }

inline const std::string&
Command::name() const { return name_; }

inline const std::string&
Command::help() const { return doc_; }

inline
Commands::Commands() {
  declare<Help_command>(*this);
}

inline
Commands::~Commands() {
  for (auto p : *this)
    delete p.second;
}

template<typename T, typename... Args>
  inline Command*
  Commands::declare(Args&&... args) {
    Command* cmd = new T(std::forward<Args>(args)...);
    (*this)[cmd->name()] = cmd;
    return cmd;
  }


// -------------------------------------------------------------------------- //
// Location

// Constructor
inline
Location::Location(const Source& s) : source_(s) { }

inline void 
Location::set_source(const Source& s) {
  source_ = s;
}

inline Source 
Location::get_source() const {
  return source_;
}



// -------------------------------------------------------------------------- //
// Arguments

// Constructor
inline
Argument::Argument(const json::Value& v, const Source& s) 
  : value_(v), loc_(s) 
{ }

// Mutators
inline void 
Argument::set_value(const json::Value& v) {
  value_ = v;
}

inline void 
Argument::set_source(const Source& s) {
  loc_.set_source(s);
}

// Accessors
inline json::Value 
Argument::get_value() const {
  return value_;
}

inline Source 
Argument::get_source() const {
  return loc_.get_source();
}

inline bool 
Arguments::has_named(const Parameter& parm) {
  if (named_.count(parm.name()))
    return true;
  else
    return false;
}

inline bool 
Arguments::has_named(const std::string& n) {
  if (named_.count(n))
    return true;
  else
    return false;
}

inline void
Arguments::display_errors(const char* pre) {
  std::stringstream errors;
  for (const auto & arg : named_) {
    if (arg.second.get_value().type() == json::Value::ERROR) {
      switch (arg.second.get_source()) {
        case ENVIRONMENT: {
          errors << "error: environment variable '" 
                 << make_env_var(pre, arg.first) << "' " 
                 <<  display_err_info(arg.second.get_value()/*.as_error*/) << "\n";
          break;
        }
        case CONFIG: {
          errors << "error: argument '" << arg.first << "' from config file " 
                 << display_err_info(arg.second.get_value()/*.as_error*/) << "\n";
          break;
        }
        case COMMAND_LINE: {
          errors << "error: command-line argument '" << arg.first << "' " 
                 << display_err_info(arg.second.get_value()/*.as_error*/) << "\n";
          break;
        }
        case FROM_DEFAULT: {
          errors << "error: default argument '" << arg.first << "' " 
                 << display_err_info(arg.second.get_value()/*.as_error*/) << "\n";
          break;
        }
        case NOT_PROVIDED: {
          errors << "error: argument '" << arg.first << "' " 
                 <<  display_err_info(arg.second.get_value()/*.as_error*/) << "\n";
        }
      }
    }
  }
  std::cerr << errors.str();
}

inline std::string
display_err_info(const json::Value& v) { 
  Error err = v.as_error();
  return err.message();
}

// Mutators

inline void
Arguments::set_named(const Parameter& parm, const Argument& arg) {
  if (this->has_named(parm)) // argument exists already
    named_[parm.name()] = arg;
  else 
    named_.emplace(parm.name(), arg);
}

inline void
Arguments::set_named(const std::string& n, const Argument& arg) {
  if (this->has_named(n)) // argument exists already
    named_[n] = arg;
  else 
    named_.emplace(n, arg); 
}

inline void
Arguments::set_listed(const json::Value& v) {
  listed_.push_back(v);
}

// Accessors

/// Returns the string pointed to by the name passed in the function. If the
/// name does not exist in the map then ...
inline json::Value 
Arguments::get_named_value(const std::string& n) const {
  return named_.find(n)->second.get_value();
}

inline Argument
Arguments::get_named_arg(const std::string& arg) const {
  return named_.find(arg)->second;
}

inline json::Value
Arguments::get_listed(const int& i) const {
  return listed_[i];
}

inline int 
Arguments::get_listed_size() const {
  return listed_.size();
}


// -------------------------------------------------------------------------- //
// Type checkers

namespace {
inline Error
make_type_error() { return make_error_code(json::errc::TYPE_ERROR); }
} // namespace

inline json::Value 
Null_typed::operator()(const json::Value& s) const {
  if (s == "null" or s=="")
    return {};
  else
    return make_type_error();
  return {};
}

inline json::Value 
Bool_typed::operator()(const json::Value& s) const {
  if (s == "true")
    return true;
  else if (s == "false")
    return false;
  else 
    return make_type_error();
  return {};
}

inline json::Value
Int_typed::operator()(const json::Value& s) const {
  return {};
}

inline json::Value 
Real_typed::operator()(const json::Value& s) const {
  double d;
  // FIXME: Validate the s is a string.
  std::stringstream ss(s.as_string());
  if (ss >> d) 
    return d;
  else 
    return make_type_error();
}

inline json::Value
String_typed::operator()(const json::Value& s) const {
  return s;
}

template<typename T>
  inline json::Value 
  Optional_typed<T>::operator()(const json::Value& s) const {
    T type;
    Null_typed null;
    if (json::Value x = null(s))
      return x;
    return type(s);
  }

template<typename T>
  inline json::Value
  Sequence_typed<T>::operator()(const json::Value& s) const {
    return {};
  }


// -------------------------------------------------------------------------- //
// Parsing

inline
Parse_state::Parse_state(const int& count, const int& curr, char* args[])
  : argc(count), current(curr), argv(args) { }


} // namespace cli
} // namespace freeflow
