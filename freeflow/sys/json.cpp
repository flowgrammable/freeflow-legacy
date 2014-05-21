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
#include <cctype>
#include <iostream>
#include <sstream>

#include <freeflow/sys/json.hpp>
#include <freeflow/sys/file.hpp>
#include <freeflow/sys/buffer.hpp>

namespace freeflow {
namespace json {

namespace {

// Returns true if c starts a keyword or identifier.
inline bool
is_id_init(char c) { return std::isalpha(c) || c == '_'; }

// Returns true if c is a character in a keyword or identifier.
inline bool
is_id_rest(char c) { return std::isalnum(c) || c == '_'; }

// Returns true if c starts a number.
inline bool
is_num_init(char c) { return std::isdigit(c) || c == '-'; }


Value parse_value(const char*&, const char*);

void
parse_ws(const char*& first, const char* last) {
  while (first != last && std::isspace(*first))
    ++first;
}

// Parse a keyword or identifier. An identifier is simply
// a string.
Value
parse_id(const char*& first, const char* last) {
  const char* init = first++;
  while (first != last and is_id_rest(*first))
    ++first;
  String id(init, first);
  if (id == "null")
    return Value{};
  if (id == "true")
    return Value(true);
  if (id == "false")
    return Value(false);
  return Value(std::move(id));
}

template<typename T>
  inline T
  convert(const char* first, const char* last) {
    std::string s(first, last);
    std::stringstream ss(s);
    T n;
    ss >> n;
    return n;
  }

// Advance as long as we have digits.
void
parse_digits(const char*& first, const char* last) {
  while (first != last && std::isdigit(*first))
    ++first;
}

// Parse an int or real.
//
// TODO: Implement parsing for real numbers.
// TODO: Provide hooks for parsing hex, binary, and ip addresses.
Value
parse_num(const char*& first, const char* last) {
  const char* init = first++; // Consume the first digit (or '-').

  // Parse the integer part.
  parse_digits(first, last);
  return Value(convert<Int>(init, first));
}

// Parse a string. Note that the resulting value will contain
// the quotation marks.
//
// TODO: Support raw string literals?
Value
parse_string(const char*& first, const char* last) {
  const char* init = first++; // Consumes the '"'
  while (first != last) {
    // Handle escape characters
    if (*first == '\\') {
      if (first == last)
        throw std::runtime_error("invalid string");
      first += 2; // Skip the escape charater
    }
    // Find the end quote.
    if (*first == '"') {
      ++first;
      break;
    }
    ++first;
  }
  return String(init, first);
}

Value
parse_array(const char*& first, const char* last) {
  ++first; // Consume the '['
  
  Array a;
  while (1) {
    // Parse the next value in the list.
    a.push_back(parse_value(first, last));

    parse_ws(first, last);
    if (first == last)
      throw std::runtime_error("invalid array");

    // If there's a comma, continue.
    if (*first == ',') {
      ++first;
      continue;
    }

    // If there's a closing bracket, stop.
    if (*first == ']') {
      ++first;
      break;
    }

    // If there's anything else, fail.
    throw std::runtime_error("array parse error");
  }
  return a;
}

// Parse a key-value pair.
Pair
parse_pair(const char*& first, const char* last) {
  // Parse and check the key.
  Value k = parse_value(first, last);
  if (k.type() != Value::STRING)
    throw std::runtime_error("key is not a string");

  parse_ws(first, last);
  if (*first != ':')
    throw std::runtime_error("pair parse error");
  ++first;

  // Parse the value.
  Value v = parse_value(first, last);

  return Pair(std::move(k.as_string()), std::move(v));
}

// Parse an object.
Value
parse_object(const char*& first, const char* last) {
  ++first; // Consume the '{'
  
  Object o;
  while (1) {
    // Parse the next pair in the set.
    o.insert(parse_pair(first, last));

    parse_ws(first, last);
    if (first == last)
      throw std::runtime_error("invalid array");

    // If there's a comma, continue.
    if (*first == ',') {
      ++first;
      continue;
    }

    // If there's a closing brace, stop.
    if (*first == '}') {
      ++first;
      break;
    }

    // If there's anything else, fail.
    throw std::runtime_error("object parse error");
  }
  return o;
}

Value
parse_value(const char*& first, const char* last) {
  parse_ws(first, last);
  switch (*first) {
  case '"': 
    return parse_string(first, last);
  case '[': 
    return parse_array(first, last);
  case '{': 
    return parse_object(first, last);
  default:
    if (is_id_init(*first))
      return parse_id(first, last);
    if (is_num_init(*first))
      return parse_num(first, last);

    // FIXME: Integrate this with usual error system.
    throw std::runtime_error("invalid input");
  } // switch (*first)
}
} // namespace

// FIXME: We should really be using a streaming framework for reading
// and writing JSON. Unfortunately, the C++ streams framework is
// not terribly efficient and not adapted for sockets or pipes. We
// need to build a new streaming library over our Resource model.

Value
parse(File& f) {
  // Read the entire file into memory and then parse it.
  // FIXME: If the parser were stream-based, we could a) avoid a single
  // alloc/read, and b) make this work for resources, not just files.
  std::size_t n = f.size();
  char* buf = new char[n];
  f.read(buf, n);
  const char* p = buf;
  Value v = parse_value(p, p + n);
  delete buf;
  return v;
}

/// Parse JSON data from the given buffer.
Value
parse(Buffer& buf) {
  const char* first = reinterpret_cast<const char*>(&buf[0]);
  return parse_value(first, first + buf.size());
}

/// Parse JSON data from the given buffer.
Value
parse(Buffer& buf, int n) {
  const char* first = reinterpret_cast<const char*>(&buf[0]);
  return parse_value(first, first + n);
}

Value
parse(const std::string& str) {
  const char* first = str.c_str();
  return parse_value(first, first + str.size());
}


} // namespace json
} // namespace freeflow
