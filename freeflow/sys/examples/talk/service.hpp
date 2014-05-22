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

#ifndef SERVICE_HPP
#define SERVICE_HPP

#include <iostream>

// This is a collection of utilities used to help write various
// services.

// -------------------------------------------------------------------------- //
// Printing support

struct brackets {
  static char open() { return '['; }
  static char close() { return ']'; }
};

struct braces {
  static char open() { return '{'; }
  static char close() { return '}'; }
};

struct parens {
  static char open() { return '('; }
  static char close() { return ')'; }
};

template<typename E, typename T>
  struct enclosure_printer {
    enclosure_printer(const T& x) 
      : obj(x) { }
    
    const T& obj;
  };

// Enclose x in brackets.
template<typename T>
  inline enclosure_printer<brackets, T>
  bracket(const T& x) { return enclosure_printer<brackets, T>(x); }


template<typename C, typename T, typename E, typename X>
  inline std::basic_ostream<C, T>&
  operator<<(std::basic_ostream<C, T>& os, const enclosure_printer<E, X>& p) {
    return os << E::open() << p.obj << E::close();
  }


#endif
