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

namespace freeflow {
namespace ofp {

// -------------------------------------------------------------------------- //
// Primitives

// Bytes

/// Returns the number of bytes required by the integral type. This
/// is the same as its size.
template<typename T, typename = Requires<Integral<T>()>>
  constexpr std::size_t 
  bytes(T) { return sizeof(T); }

/// Returns the number of bytes in the array. 
template<typename T, std::size_t N>
  constexpr std::size_t
  bytes(const T(&a)[N]) { return bytes(a[0]) * N; }

inline std::size_t
bytes(const Buffer& b) { return b.size(); }

constexpr std::size_t 
bytes(const Mac_addr&) { return 6; }

constexpr std::size_t 
bytes(const Ipv4_addr&) { return 4; }

constexpr std::size_t 
bytes(const Ipv6_addr&) { return 16; }

constexpr std::size_t 
bytes(const Header&) { return 8; }

// To view

inline Error
to_view(View& v, Uint8 n) {
  put(v, n);
  return {};
}

inline Error
to_view(View& v, Uint16 n) {
  put(v, Byte_order::msbf(n));
  return {};
}

inline Error
to_view(View& v, Uint32 n) {
  put(v, Byte_order::msbf(n));
  return {};
}

inline Error
to_view(View& v, Uint64 n) {
  put(v, Byte_order::msbf(n));
  return {};
}

template<typename T, typename X>
  inline Error
  to_view(View& v, T value) {
    using U = Underlying_type<T>;
    put(v, Byte_order::msbf(static_cast<U>(value)));
    return {};
  }

template<typename T, std::size_t N>
  inline Error 
  to_view(View& v, T(&a)[N]) {
    put(v, a, sizeof(T[N]));
    return {};
  }

/// Write the contents of the buffer into the view. Note that no
/// transformations are applied to the contents of the buffer.
inline Error
to_view(View& v, const Buffer& b) {
  put(v, b.data(), b.size());
  return {};
}

inline Error
from_view(View& v, Uint8& n) {
  n = Byte_order::msbf(get<Uint8>(v));
  return {};
}

inline Error
from_view(View& v, Uint16& n) {
  n = Byte_order::msbf(get<Uint16>(v));
  return {};
}

inline Error
from_view(View& v, Uint32& n) {
  n = Byte_order::msbf(get<Uint32>(v));
  return {};
}

inline Error
from_view(View& v, Uint64& n) {
  n = Byte_order::msbf(get<Uint64>(v));
  return {};
}

template<typename T, typename X>
  inline Error
  from_view(View& v, T& value) {
    using U = Underlying_type<T>;
    value = static_cast<T>(Byte_order::msbf(get<U>(v)));
    return {};
  }

template<typename T, std::size_t N>
  inline Error 
  from_view(View& v, T(&a)[N]) {
    get(v, a, sizeof(T[N]));
    return {};
  }

/// Copy the contents from the view into the buffer. The buffer is
/// resized to accommodate the remaining contents of the view. Note 
/// that no transformations are applied to the contents of the buffer.
inline Error
from_view(View& v, Buffer& b) {
  b.resize(v.remaining());
  get(v, b.data(), b.size());
  return {};
}

// -------------------------------------------------------------------------- //
// Common structures

inline Error 
to_view(View& v, const Mac_addr& a) { return to_view(v, a.addr); }

inline Error 
to_view(View& v, const Ipv4_addr& a) { return to_view(v, a.addr); }

inline Error 
to_view(View& v, const Ipv6_addr& a) { return to_view(v, a.addr); }

inline Error 
from_view(View& v, Mac_addr& a) { return from_view(v, a.addr); }

inline Error 
from_view(View& v, Ipv4_addr& a) { return from_view(v, a.addr); }

inline Error 
from_view(View& v, Ipv6_addr& a) { return from_view(v, a.addr); }


} // namespace ofp
} // namespace freeflow
