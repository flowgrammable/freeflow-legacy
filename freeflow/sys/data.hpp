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

#include <endian.h>

#include <cstdint>

#include <freeflow/sys/meta.hpp>

namespace freeflow {

// -------------------------------------------------------------------------- //
// Data types

using Uint8 = std::uint8_t;
using Uint16 = std::uint16_t;
using Uint32 = std::uint32_t;
using Uint64 = std::uint64_t;


// -------------------------------------------------------------------------- //
// Byte order

// There's no portable way to do this yet. Note that these macros
// are used only within the Byte_order class. 
#if defined(FREEFLOW_BIG_ENDIAN)
#  define ff_msbf_16(x) x
#  define ff_lsbf_16(x) __builtin_bswap16(x)
#  define ff_msbf_32(x) x
#  define ff_lsbf_32(x) __builtin_bswap32(x)
#  define ff_msbf_64(x) x
#  define ff_lsbf_64(x) __builtin_bswap64(x)
#elif defined(FREEFLOW_LITTLE_ENDIAN)
#  define ff_msbf_16(x) __builtin_bswap16(x)
#  define ff_lsbf_16(x) x
#  define ff_msbf_32(x) __builtin_bswap32(x)
#  define ff_lsbf_32(x) x
#  define ff_msbf_64(x) __builtin_bswap64(x)
#  define ff_lsbf_64(x) x
#endif

/// The Byte_order class provides facilities for reordering the bytes
/// in unsigned integral values so that the most or least significant bit
/// appears first in the byte sequence.
struct Byte_order {
  static Uint8 msbf(Uint8 v) { return v; }
  static Uint16 msbf(Uint16 v) { return ff_msbf_16(v); }
  static Uint32 msbf(Uint32 v) { return ff_msbf_32(v); }
  static Uint64 msbf(Uint64 v) { return ff_msbf_64(v); }

  static Uint8 lsbf(Uint8 v) { return v; }
  static Uint16 lsbf(Uint16 v) { return ff_lsbf_16(v); }
  static Uint32 lsbf(Uint32 v) { return ff_lsbf_16(v); }
  static Uint64 lsbf(Uint64 v) { return ff_lsbf_16(v); }
};

} // namespace freeflow

#endif
