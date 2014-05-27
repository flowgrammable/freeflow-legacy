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


namespace cli {

// -------------------------------------------------------------------------- //
// Parameter

inline
Parameter::Parameter(const std::string& n, 
                     const Type& t, 
                     const Initializer& i, 
                     const std::string& d)
  : name_{n, {}}, type_(t), init_(i), doc_(d) { }

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
inline const Parameter::Initializer&
Parameter::init() const { return init_; }

/// Returns the string containing the documentation for the parameter. 
inline const std::string&
Parameter::doc() const { return doc_; }


// -------------------------------------------------------------------------- //
// Type checking

inline
Value::Value(Error)
  : ff::json::Value(), error_(true) { }

inline 
Value::operator bool() const { return not error_; }

// -------------------------------------------------------------------------- //
// Type checkers

inline Value 
Null::operator()(const std::string& s) const {
  if (s == "null")
    return {};
  else
    return Value::error;
}

inline Value 
Bool::operator()(const std::string& s) const {
  if (s == "true")
    return true;
  else if (s == "false")
    return false;
  else 
    return Value::error;
}

inline Value
Int::operator()(const std::string& s) const {
  return {};
}

inline Value 
Real::operator()(const std::string& s) const {
  double d;
  std::stringstream ss(s);
  if (ss >> d) 
    return d;
  else 
    return Value::error;
}

inline Value
String::operator()(const std::string& s) const {
  return {};
}

template<typename T>
  inline Value 
  Optional<T>::operator()(const std::string& s) const {
    T type;
    Null null;
    if (Value x = null(s))
      return x;
    return type(s);
  }

template<typename T>
  inline Value
  Sequence<T>::operator()(const std::string& s) const {
    return {};
  }

} // namespace cli
