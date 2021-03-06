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

#ifndef FREEFLOW_META_HPP
#define FREEFLOW_META_HPP

#include <type_traits>

namespace freeflow {

/// Names the type T if B is true, or otherwise causes a substitution
/// failure.
template<bool B, typename T = void>
  using Requires = typename std::enable_if<B, T>::type;

/// True if T is an intgral type, including enumerations.
template<typename T>
  constexpr bool Integral() { return std::is_integral<T>::value; }

/// True if T is an enum type.
template<typename T>
  constexpr bool Enum() { return std::is_enum<T>::value; }

/// True if T is a literal type.
template<typename T>
  constexpr bool Literal() { return std::is_literal_type<T>::value; }


template<typename T, typename U>
  constexpr bool Derived_from() { return std::is_base_of<U, T>::value; }


/// The underlying integral type of an enum.
template<typename T>
  using Underlying_type = typename std::underlying_type<T>::type;

} // namespace freeflow

#endif
