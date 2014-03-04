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

#ifndef FREEFLOW_DATA_HPP
#define FREEFLOW_DATA_HPP

#include <cstdint>

#include <freeflow/sys/meta.hpp>

namespace freeflow {

/// Basic data types
using Uint8 = std::uint8_t;
using Uint16 = std::uint16_t;
using Uint32 = std::uint32_t;
using Uint64 = std::uint64_t;


// -------------------------------------------------------------------------- //
// Foreign Byte Order

/// The Foreign_byte_order class provides facilities for reordering the bytes
/// in unsigned integral values so that the most or least significant bit
/// appears first in the byte sequence.
///
/// \todo Clang will not compile this class with constexpr byte-ordering
/// functions as it is written. The reason is that the test against value
/// is not the active member of the union. Fixing this should rely on 
/// config-time information, and then write this as constexprs. For now, they 
/// are made non-constexpr.
struct Byte_order {
  // Union and initial value to test underlying architecture
  union Test { 
    Uint8 a[4]; 
    Uint32 value; 
  };
  static constexpr Test test = {{ 1, 2, 3, 4 }};
  
  // Test values to read underlying architecture behavior.
  static constexpr Uint32 MSBF = 0x01020304;
  static constexpr Uint32 LSBF = 0x04030201;

  static constexpr bool is_msbf() { return test.value == MSBF; }
  static constexpr bool is_lsbf() { return test.value == LSBF; }
  
  // Convert the foreign representation to most-significant-byte-first.
  static Uint8 msbf(Uint8 v) { return v; }
  static Uint16 msbf(Uint16 v) { return is_msbf() ? v : __builtin_bswap16(v); }
  static Uint32 msbf(Uint32 v) { return is_msbf() ? v : __builtin_bswap32(v); }
  static Uint64 msbf(Uint64 v) { return is_msbf() ? v : __builtin_bswap64(v); }

  template<typename T>
    static T msbf(T v) { return (T)msbf(Underlying_type<T>(v)); }
  

  // Convert the foreign representation to least-significant-byte-first.
  static Uint8 lsbf(Uint8 v) { return v; }
  static Uint16 lsbf(Uint16 v) { return is_lsbf() ? v : __builtin_bswap16(v); }
  static Uint32 lsbf(Uint32 v) { return is_lsbf() ? v : __builtin_bswap32(v); }
  static Uint64 lsbf(Uint64 v) { return is_lsbf() ? v : __builtin_bswap64(v); }

  template<typename T>
    static T lsbf(T v) { return lsbf(Underlying_type<T>(v)); }
};


} // namespace freeflow

#endif
