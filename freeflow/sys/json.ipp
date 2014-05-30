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

#include <cassert>

namespace freeflow {
namespace json {

inline
Error::Error(Error_code c, intptr_t d) : code(c), data(d) { } 

/// Allow implicit convesion from std::string.
inline
String::String(std::string&& s)
  : std::string(std::move(s)) { }

/// Allow implicit conversion from std::string.
inline
String::String(const std::string& s)
  : std::string(s) { }

/// Returns true if the string is quoted.
inline bool
String::is_quoted() const {
  if (not empty())
    return (*this)[0] == '"';
}

inline
Value::Value()
  : type_(NIL), data_() { }

inline
Value::Value(Value&& x)
  : type_(x.type_) { move(std::move(x)); }

inline Value&
Value::operator=(Value&& x) {
  if (&x != this) {
    destroy();
    type_ = x.type_;
    move(std::move(x));
  }
  return *this;
}

inline
Value::Value(const Value& x)
  : type_(x.type_) { copy(x); }

inline Value&
Value::operator=(const Value& x) {
  if (&x != this) {
    destroy();
    type_ = x.type_;
    copy(x);
  }
  return *this;
}

inline
Value::operator bool() {
  return type_ != ERROR;
}

// Value construction
inline
Value::Value(Null n)
  : type_(NIL), data_(n) { }

inline
Value::Value(std::nullptr_t)
  : type_(NIL), data_(Null{}) { }

inline
Value::Value(Bool b)
  : type_(BOOL), data_(b) { }

inline
Value::Value(int n)
  : type_(INT), data_(Int(n)) { }

inline
Value::Value(Int z)
  : type_(INT), data_(z) { }
  
inline
Value::Value(const Error e)
  : type_(ERROR), data_(e) { }

inline
Value::Value(Real r)
  : type_(REAL), data_(r) { }

inline
Value::Value(String&& s)
  : type_(STRING), data_(std::move(s)) { }

inline
Value::Value(const char* s)
  : type_(STRING), data_(String(s)) { }

inline
Value::Value(std::string&& s)
  : type_(STRING), data_(std::move(s)) { }

inline
Value::Value(const std::string& s)
  : type_(STRING), data_(s) { }

inline
Value::Value(const String& s)
  : type_(STRING), data_(s) { }

inline
Value::Value(Array&& a)
  : type_(ARRAY), data_(std::move(a)) { }

inline
Value::Value(const Array& a)
  : type_(ARRAY), data_(a) { }

inline
Value::Value(Object&& o)
  : type_(OBJECT), data_(std::move(o)) { }

inline
Value::Value(const Object& o)
  : type_(OBJECT), data_(o) { }

inline
Value::~Value() { destroy(); }

inline Value::Type
Value::type() const { return type_; }

// Copy the contents of x into this value. Note that type_ is already
// initialized or assigned.
inline void
Value::copy(const Value& x) {
  switch (type_) {
  case NIL: new (&data_.n) Null(x.data_.n); break;
  case BOOL: new (&data_.b) Bool(x.data_.b); break;
  case INT: new (&data_.z) Int(x.data_.z); break;
  case REAL: new (&data_.r) Real(x.data_.r); break;
  case STRING: new (&data_.s) String(x.data_.s); break;
  case ARRAY: new (&data_.a) Array(x.data_.a); break;
  case OBJECT: new (&data_.o) Object(x.data_.o); break;
  case ERROR: new (&data_.e) Error(x.data_.e); break;
  }
}

// Move the contents of x into this value. Note that type_ is already
// initialized or assigned.
inline void
Value::move(Value&& x) {
  switch (type_) {
  case NIL: new (&data_.n) Null(x.data_.n); break;
  case BOOL: new (&data_.b) Bool(x.data_.b); break;
  case INT: new (&data_.z) Int(x.data_.z); break;
  case REAL: new (&data_.r) Real(x.data_.r); break;
  case STRING: new (&data_.s) String(std::move(x.data_.s)); break;
  case ARRAY: new (&data_.a) Array(std::move(x.data_.a)); break;
  case OBJECT: new (&data_.o) Object(std::move(x.data_.o)); break;
  case ERROR: new (&data_.e) Error(x.data_.e); break;
  }
}

// Destroy non-trivial data_ members.
inline void
Value::destroy() {
  switch(type_) {
  case NIL: data_.n.~Null(); break;
  case BOOL: data_.b.~Bool(); break;
  case INT: data_.z.~Int(); break;
  case REAL: data_.r.~Real(); break;
  case STRING: data_.s.~String(); break;
  case ARRAY: data_.a.~Array(); break;
  case OBJECT: data_.o.~Object(); break;
  case ERROR: data_.e.~Error(); break;
  default: break;
  }
}

template<typename T>
  inline T&
  Value::check(Type t, T& x) {
    assert(type_ == t);
    return x;
  }

template<typename T>
  inline const T&
  Value::check(Type t, const T& x) const {
    assert(type_ == t);
    return x;
  }

inline Null&
Value::as_null(){ return check(NIL, data_.n); }

inline const Null&
Value::as_null() const { return check(NIL, data_.n); }

inline Bool&
Value::as_bool() { return check(BOOL, data_.b); }

inline const Bool&
Value::as_bool() const { return check(BOOL, data_.b); }

inline Int&
Value::as_int() { return check(INT, data_.z); }

inline const Int&
Value::as_int() const { return check(INT, data_.z); }

inline Real&
Value::as_real() { return check(REAL, data_.r); }

inline const Real&
Value::as_real() const { return check(REAL, data_.r); }

inline String&
Value::as_string() { return check(STRING, data_.s); }

inline const String&
Value::as_string() const { return check(STRING, data_.s); }

inline Array&
Value::as_array() { return check(ARRAY, data_.a); }

inline const Array&
Value::as_array() const { return check(ARRAY, data_.a); }

inline Object&
Value::as_object() { return check(OBJECT, data_.o); }

inline const Object&
Value::as_object() const { return check(OBJECT, data_.o); }

// TODO: Implement a pretty printer for JSON. It would be nice
// if we could integrate use the same pretty printing context
// for messages, resources, and JSON.

template<typename C, typename T>
  inline std::basic_ostream<C, T>&
  operator<<(std::basic_ostream<C, T>& os, const Pair& p) {
    return os << p.first << ':' << p.second;
  }

namespace {
template<typename C, typename T, typename X>
  inline void
  print_composite(std::basic_ostream<C, T>& os, const X& x) {
    for (auto i = x.begin(); i != x.end(); ++i) {
      os << *i;
       if (std::next(i) != x.end())
        os << ',';
    }
  }

template<typename C, typename T>
  inline std::basic_ostream<C, T>&
  print(std::basic_ostream<C, T>& os, const Array& a) {
    os << '['; 
    print_composite(os, a);
    os  << ']';
    return os;
  }

template<typename C, typename T>
  inline std::basic_ostream<C, T>&
  print(std::basic_ostream<C, T>& os, const Object& a) {
    os << '{'; 
    print_composite(os, a);
    os  << '}';
    return os;
  }
} // namespace

template<typename C, typename T>
  inline std::basic_ostream<C, T>&
  operator<<(std::basic_ostream<C, T>& os, const Value& v) {
    switch (v.type()) {
    case Value::NIL:
      return os << "null";
    case Value::BOOL:
      return os << (v.as_bool() ? "true" : "false");
    case Value::INT:
      return os << v.as_int();
    case Value::REAL:
      return os << v.as_real();
    case Value::STRING:
      return os << v.as_string();
    case Value::ARRAY:
      return print(os, v.as_array());
    case Value::OBJECT:
      return print(os, v.as_object());
    case Value::ERROR:
       return os << "ERROR!"; // TODO: smarter error messages?
    }
    return os;
  }


} // namespace json
} // namespace freeflow
