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

#ifndef FREEFLOW_JSON_HPP
#define FREEFLOW_JSON_HPP

#include <iosfwd>
#include <string>
#include <map>
#include <vector>

#include <freeflow/sys/data.hpp>

namespace freeflow {

class File;
class Buffer;

namespace json {

class Value;

/// Represents the type of the null literal in JSON.
struct Null { };

/// Represents the type of the true and false literals in JSON.
using Bool = bool;

/// Represents the type of an integer value in JSON. Note that JSON the
/// specification does not distinguish between integers and real numbers,
/// but we make that distinction here.
using Int = Int64;

/// Represents the type of a real value in JSON. Note that JSON the
/// specification does not distinguish between integers and real numbers,
/// but we make that distinction here.
using Real = double;

/// Represents the type of a string value in JSON. This string 
/// representation is limited to 8-bit characters (they may be UTF-8)
/// encoded.
///
/// \todo This type does not provide an interpretation of escape characters.
/// It should. This is to say that when parsing, if we encounter a '\t',
/// that sequence should be replaced by a tab character.
struct String : std::string {
  using std::string::string;

  String(std::string&&);
  String(const std::string&);

  bool is_quoted() const;
};

/// Represents an array of values.
using Array = std::vector<Value>;

/// Represents a set of key-value pairs. Note that the set is sorted.
///
/// We use a sorted representation only because a std::unordered_map cannot
/// be declared with a mapped type that is incomplete.
using Object = std::map<String, Value>;

/// A Key-value pair is a sub-object of an Object. This is not a JSON
/// value, it is provided only for the convenience of working with
/// Objects.
using Pair = std::pair<String, Value>;


/// The value class represents an abstract JSON value. It is one of the
/// types specified above.
///
/// \todo The set of values can be readily extended with new literal types.
/// For example, it might be useful to include ipv4 and ipv6 addresses,
/// hex literals, or binary literals as new kinds of values.

enum Error_code {
  TYPE_ERROR,
  PARSE_ERROR,
  REQUIRED_ERROR,
  VALUE_ERROR
};


struct Error {
  Error_code code;
  intptr_t   data;
  
  Error(Error_code, intptr_t);
};

class Value {
public:
  enum Type {
      NIL,
      BOOL,
      INT,
      REAL,
      STRING,
      ARRAY,
      OBJECT,
      ERROR
  };

  union Data {
    Data() : n() { }
    Data(Null n) : n(n) { }
    Data(Bool b) : b(b) { }
    Data(Int z) : z(z) { }
    Data(Real r) : r(r) { }
    Data(String&& s) : s(std::move(s)) { }
    Data(const String& s) : s(s) { }
    Data(Array&& a) : a(std::move(a)) { }
    Data(const Array& a) : a(a) { }
    Data(Object&& o) : o(std::move(o)) { }
    Data(const Object& o) : o(o) { }
    Data(Error _e) : e(_e) { }
    ~Data() { }

    Null   n;
    Bool   b;
    Int    z;
    Real   r;
    String s;
    Array  a;
    Object o;
    Error  e;
  };

  Value();

  // Move semantics
  Value(Value&&);
  Value& operator=(Value&&);

  // Copy semantics
  Value(const Value&);
  Value& operator=(const Value&);

  // Value construction
  Value(Null);
  Value(std::nullptr_t);
  Value(Bool);
  Value(int);
  Value(Int);
  Value(Real);
  Value(const char*);
  Value(std::string&&);
  Value(const std::string&);
  Value(String&&);
  Value(const String&);
  Value(Array&&);
  Value(const Array&);
  Value(Object&&);
  Value(const Object&);
  Value(const Error e);

  ~Value();

  Type type() const;

  Null&       as_null();
  const Null& as_null() const;
  
  Bool&       as_bool();
  const Bool& as_bool() const;
  
  Int&       as_int();
  const Int& as_int() const;
  
  Real&       as_real();
  const Real& as_real() const;
  
  String&       as_string();
  const String& as_string() const;
  
  Array&       as_array();
  const Array& as_array() const;
  
  Object&       as_object();
  const Object& as_object() const;
  
  operator bool();

private:
  void copy(const Value&);
  void move(Value&&);
  void destroy();

  template<typename T> T&       check(Type, T&);
  template<typename T> const T& check(Type, const T&) const;

  Type type_;
  Data data_;
};

// JSON parsing.
Value parse(File&);
Value parse(Buffer&);
Value parse(Buffer&, std::size_t);
Value parse(const std::string&);

void write(File& r, const Value&);

// Streaming
template<typename C, typename T>
  std::basic_ostream<C, T>&
  operator<<(const std::basic_ostream<C, T>&, const Value&);

} // namespace json
} // namespace freeflow

#include <freeflow/sys/json.ipp>

#endif
