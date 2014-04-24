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
Value::Value()
  : type_(NIL), data_() { }

inline
Value::Value(Value&& x)
  : type_(x.type_) 
{
  switch (type_) {
  case NIL: new (&data_.n) Null(x.data_.n); break;
  case BOOL: new (&data_.b) Bool(x.data_.b); break;
  case INT: new (&data_.z) Int(x.data_.z); break;
  case REAL: new (&data_.r) Real(x.data_.r); break;
  case STRING: new (&data_.s) String(std::move(x.data_.s)); break;
  case ARRAY: new (&data_.a) Array(std::move(x.data_.a)); break;
  case OBJECT: new (&data_.o) Object(std::move(x.data_.o)); break;
  }
}

inline
Value::Value(const Value& x)
  : type_(x.type_)
{
  switch (type_) {
  case NIL: new (&data_.n) Null(x.data_.n); break;
  case BOOL: new (&data_.b) Bool(x.data_.b); break;
  case INT: new (&data_.z) Int(x.data_.z); break;
  case REAL: new (&data_.r) Real(x.data_.r); break;
  case STRING: new (&data_.s) String(x.data_.s); break;
  case ARRAY: new (&data_.a) Array(x.data_.a); break;
  case OBJECT: new (&data_.o) Object(x.data_.o); break;
  }
}

// Value construction
inline
Value::Value(Null n)
  : type_(NIL), data_(n) { }

inline
Value::Value(Bool b)
  : type_(BOOL), data_(b) { }

inline
Value::Value(Int z)
  : type_(INT), data_(z) { }

inline
Value::Value(Real r)
  : type_(REAL), data_(r) { }

inline
Value::Value(String&& s)
  : type_(STRING), data_(std::move(s)) { }

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
Value::~Value() {
  // Destroy non-trivial data_ members.
  switch(type_) {
  case STRING: data_.s.~String(); break;
  case ARRAY: data_.a.~Array(); break;
  case OBJECT: data_.o.~Object(); break;
  default: break;
  }
}

inline Value::Type
Value::type() const { return type_; }

template<typename T>
  inline const T&
  Value::check(Type t, const T& x) const {
    assert(type_ == t);
    return x;
  }

inline const Null&
Value::as_null() const { return check(NIL, data_.n); }

inline const Bool&
Value::as_bool() const { return check(BOOL, data_.b); }

inline const Int&
Value::as_int() const { return check(INT, data_.z); }

inline const Real&
Value::as_real() const { return check(REAL, data_.r); }

inline const String&
Value::as_string() const { return check(STRING, data_.s); }

inline const Array&
Value::as_array() const { return check(ARRAY, data_.a); }

inline const Object&
Value::as_object() const { return check(OBJECT, data_.o); }

} // namespace json
} // namespace freeflow
