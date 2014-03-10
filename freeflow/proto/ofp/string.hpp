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

#ifndef FREEFLOW_OFP_STRING_HPP
#define FREEFLOW_OFP_STRING_HPP

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/error.hpp>

namespace freeflow {
namespace ofp {

/// The String class is a statically sized C-string whose contents are
/// zero-filled. Note that this class does not provide the features typical
/// of the usual string abstraction. This is effectively an array of
/// characters.
template<std::size_t N>
  class String {
    static_assert(N > 0, "N shall not be zero");
  public:
    String() = default;

    // Value initialization
    String(const char*);
    String& operator=(const char*);

    // Observers
    bool empty() const;
    std::size_t size() const;
    std::string str() const;

    // Character access
    char  operator[](int n) const { return data[n]; }
    char& operator[](int n)       { return data[n]; }

    char data[N];
  };

// Equality comparison
template<std::size_t N>
  bool operator==(const String<N>& a, const String<N>& b);

template<std::size_t N>
  bool operator!=(const String<N>& a, const String<N>& b);

// Protocol
template<std::size_t N>
  constexpr std::size_t bytes(const String<N>&);

template<std::size_t N>
  Error to_view(View&, const String<N>&);

template<std::size_t N>
  Error from_view(View&, String<N>&);

} // namespace ofp
} // namespace freeflow

#include "string.ipp"

#endif
