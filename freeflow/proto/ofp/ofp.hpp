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

#ifndef FREEFLOW_OFP_HPP
#define FREEFLOW_OFP_HPP

#include <cstring>
#include <string>
#include <vector>

#include <freeflow/sys/data.hpp>
#include <freeflow/sys/error.hpp>
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


/// The Sequence class represents a repeated collection of objects of
/// a particular type.
template<typename T>
  struct Sequence : public std::vector<T> {
    using std::vector<T>::vector;
  };


// -------------------------------------------------------------------------- //
// Common structures

/// A MAC address is a 48-bit identifier that uniquely identifies the device.
struct Mac_addr {
  Uint8 addr[6];
};


/// An Ipv4 address is a 32-bit identifier.
struct Ipv4_addr {
  Uint8 addr[4];
};


// An Ipv6 address is a 128-bit identifier.
struct Ipv6_addr {
  Uint8 addr[16];
};

/// The OpenFlow header is found at the front of every OpenFlow message.
struct Header {
  Uint8 version;
  Uint8 type;
  Uint16 length;
  Uint32 xid;
};

// -------------------------------------------------------------------------- //
// Protocol primitives
//
// These primitive types and functions establish the basic protocol encoding
// rules and some structures common to all versions of the OpenFlow
// protocol.

// Bytes

template<typename T, typename = Requires<Integral<T>()>>
  constexpr std::size_t bytes(T);

template<typename T, std::size_t N>
  constexpr std::size_t bytes(const T(&)[N]);

template<std::size_t N>
  constexpr std::size_t bytes(const String<N>&);

template<typename T>
  std::size_t bytes(const Sequence<T>&);

std::size_t bytes(const Buffer&);
constexpr std::size_t bytes(const Mac_addr&);
constexpr std::size_t bytes(const Ipv4_addr&);
constexpr std::size_t bytes(const Ipv6_addr&);
constexpr std::size_t bytes(const Header&);

// To view

Error to_view(View&, Uint8);
Error to_view(View&, Uint16);
Error to_view(View&, Uint32);
Error to_view(View&, Uint64);

template<typename T, typename = Requires<Enum<T>()>>
  Error to_view(View&, T);

template<typename T, std::size_t N>
  Error to_view(View&, const T(&)[N]);

template<std::size_t N>
  Error to_view(View&, const String<N>&);

template<typename T>
  Error to_view(View&, const Sequence<T>&);

Error to_view(View&, const Buffer&);
Error to_view(View&, const Mac_addr&);
Error to_view(View&, const Ipv4_addr&);
Error to_view(View&, const Ipv6_addr&);
Error to_view(View&, const Header&);

// From view

Error from_view(View&, Uint8&);
Error from_view(View&, Uint16&);
Error from_view(View&, Uint32&);
Error from_view(View&, Uint64&);

template<typename T, typename = Requires<Enum<T>()>>
  Error from_view(View&, T&);

template<typename T, std::size_t N>
  Error from_view(View&, T(&)[N]);

template<std::size_t N>
  Error from_view(View&, String<N>&);

template<typename T>
  Error from_view(View&, Sequence<T>&);

Error from_view(View&, Buffer&);
Error from_view(View&, Mac_addr&);
Error from_view(View&, Ipv4_addr&);
Error from_view(View&, Ipv6_addr&);
Error from_view(View&, Header&);

} // namespace ofp
} // freeflow

#include <freeflow/proto/ofp/ofp.ipp>

#endif
