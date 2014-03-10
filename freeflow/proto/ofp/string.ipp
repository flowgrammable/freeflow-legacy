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

namespace freeflow {
namespace ofp {
namespace detail {

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
  String<N>::String(const char* s) { detail::copy_and_zero<N>(s, data_); }

template<std::size_t N>
  inline String<N>&
  String<N>::operator=(const char* s) {
    detail::copy_and_zero<N>(s, data_);
    return *this;
  }

template<std::size_t N>
  inline bool
  String<N>::empty() const { return !data_[0]; }

template<std::size_t N>
  inline std::size_t
  String<N>::size() const { return detail::strnlen<N>(data_); }

template<std::size_t N>
  inline std::string
  String<N>::str() const { return std::string(data_, data_ + size()); }

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

} // namespace ofp
} // namespace freeflow
