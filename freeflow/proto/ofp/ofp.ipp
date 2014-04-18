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

namespace detail {

// -------------------------------------------------------------------------- //
// String

// Return the string length up to N characters. Note that s must not
// be nullptr.
template<std::size_t N>
  inline std::size_t 
  strnlen(const char* s) {
    assert(s != nullptr);
    std::size_t n = 0;
    while (*s and n < N)
      ++n;
    return n;
  }

// Copy at most N characters from s into out. Zero-fill the remaining
// characters of the string. Note that s must not be nullptr.
template<std::size_t N>
  inline void
  copy_and_zero(const char* s, char* out) {
    assert(s != nullptr);
    std::size_t n = N;
    for (; *s != 0 and n != 0; --n)
      *out++ = *s++;
    for (; n != 0; --n) {
      *out++ = 0;
    }
  }
} // namespace detail


template<std::size_t N>
  inline 
  String<N>::String(const char* s) { detail::copy_and_zero<N>(s, data); }

template<std::size_t N>
  inline String<N>&
  String<N>::operator=(const char* s) {
    detail::copy_and_zero<N>(s, data);
    return *this;
  }

template<std::size_t N>
  inline bool
  String<N>::empty() const { return !data[0]; }

template<std::size_t N>
  inline std::size_t
  String<N>::size() const { return detail::strnlen<N>(data); }

template<std::size_t N>
  inline std::string
  String<N>::str() const { return std::string(data, data + size()); }

// Equality comparison
template<std::size_t N>
  inline bool
  operator==(const String<N>& a, const String<N>& b) {
    return !std::strncmp(a.data(), b.data(), N);
  }

template<std::size_t N>
  inline bool
  operator!=(const String<N>& a, const String<N>& b) {
    return !(a == b); 
  }

// -------------------------------------------------------------------------- //
// Bytes

/// Returns the number of bytes required by the empty element. This is
/// always 0.
constexpr std::size_t
bytes(const Empty&) { return 0; }

/// Returns the number of bytes required by the integral type. This
/// is the same as its size.
template<typename T, typename = Requires<Integral<T>()>>
  constexpr std::size_t 
  bytes(T) { return sizeof(T); }

/// Returns the number of bytes in the array. 
template<typename T, std::size_t N>
  constexpr std::size_t
  bytes(const T(&a)[N]) { return bytes(a[0]) * N; }

/// Returns the bytes required by the string.
template<std::size_t N>
  constexpr std::size_t 
  bytes(const String<N>&) { return N; }

/// Returns the number of bytes required by the sequence.
template<typename T>
  inline std::size_t 
  bytes(const Sequence<T>& s) {
    std::size_t n = 0;
    for (const T& x : s)
      n += bytes(x);
    return n;
  }

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

// -------------------------------------------------------------------------- //
// To view

/// Writes an empty structure to a view.
inline Error
to_view(View&, const Empty&) { return {}; }

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
  to_view(View& v, const T(&a)[N]) {
    put(v, a, sizeof(T[N]));
    return {};
  }

/// Writes a string to the view.
template<std::size_t N>
  inline Error 
  to_view(View& v, const String<N>& s) {
    assert(bytes(s) <= v.remaining());
    return to_view(v, s.data);
  }

/// Write a sequence of elements to a view.
template<typename T>
  Error 
  to_view(View& v, const Sequence<T>& s) {
    if (v.remaining() < bytes(s))
      return Error::SEQUENCE_OVERFLOW;
    for (const auto& x : s)
      to_view(v, x);
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
to_view(View& v, const Mac_addr& a) { return to_view(v, a.addr); }

inline Error 
to_view(View& v, const Ipv4_addr& a) { return to_view(v, a.addr); }

inline Error 
to_view(View& v, const Ipv6_addr& a) { return to_view(v, a.addr); }

// -------------------------------------------------------------------------- //
// From view

/// Reads an empty structure from a view.
inline Error
from_view(View&, Empty&) { return {}; }

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

/// Reads a string from the view.
template<std::size_t N>
  inline Error 
  from_view(View& v, String<N>& s) {
    assert(bytes(s) <= v.remaining());
    return from_view(v, s.data);
  }

/// Read a sequence of elements from a view. This is done by reading 
/// and constructing objects until no more can be constructed. 
///
/// Note that elements for which bytes() is not a constant expression
/// have indeterminate size until they are read. Because of this, the
/// algorithm must check each construction.
template<typename T>
  Error
  from_view(View& v, Sequence<T>& s) {
    while (v.remaining()) {
      T t;
      if (Trap err = from_view(v, t))
        return static_cast<Error>(err.error());
      s.push_back(std::move(t));
    }
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

inline Error 
from_view(View& v, Mac_addr& a) { return from_view(v, a.addr); }

inline Error 
from_view(View& v, Ipv4_addr& a) { return from_view(v, a.addr); }

inline Error 
from_view(View& v, Ipv6_addr& a) { return from_view(v, a.addr); }

} // namespace ofp
} // namespace freeflow
