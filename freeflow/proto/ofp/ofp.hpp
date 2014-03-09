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

#include <freeflow/sys/data.hpp>
#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>

namespace freeflow {
namespace ofp {

// -------------------------------------------------------------------------- //
// Common errors

struct Error : freeflow::Error {
  /// Reading or writing the header would overflow the buffer.
  static constexpr Code HEADER_OVERFLOW  = 100;
  
  /// Reading or writing the payload would overflow the buffer. 
  static constexpr Code PAYLOAD_OVERFLOW  = 101;

  /// The protocol version is unsupported.
  static constexpr Code BAD_VERSION       = 102;

  /// The header length is less than the size of the header.
  static constexpr Code BAD_HEADER_LENGTH = 103;

  using freeflow::Error::Error;
};

// -------------------------------------------------------------------------- //
// Protocol primitives
//
// These primitive types and functions establish the basic protocol encoding
// rules and some structures common to all versions of the OpenFlow
// protocol.

Error to_view(View& v, Uint8 n);
Error to_view(View& v, Uint16 n);
Error to_view(View& v, Uint32 n);
Error to_view(View& v, Uint64 n);

template<typename T, typename = Requires<Enum<T>()>>
  Error to_view(View& v, T value);

template<typename T, std::size_t N>
  Error to_view(View& v, T(&)[N]);

Error from_view(View& v, Uint8& n);
Error from_view(View& v, Uint16& n);
Error from_view(View& v, Uint32& n);
Error from_view(View& v, Uint64& n);

template<typename T, typename = Requires<Enum<T>()>>
  Error from_view(View& v, T& value);


// -------------------------------------------------------------------------- //
// Common structures

/// A MAC address is a 48-bit identifier that uniquely identifies the device.
struct Mac_addr {
  static constexpr std::size_t bytes = 6;
  Uint8 addr[6];
};

/// An Ipv4 address is a 32-bit identifier.
struct Ipv4_addr {
  static constexpr std::size_t bytes = 4;
  Uint8 addr[4];
};

// An Ipv6 address is a 128-bit identifier.
struct Ipv6_addr {
  static constexpr std::size_t bytes = 8;
  Uint8 addr[16];
};

Error to_view(View&, const Mac_addr&);
Error to_view(View&, const Ipv4_addr&);
Error to_view(View&, const Ipv6_addr&);

Error from_view(View&, Mac_addr&);
Error from_view(View&, Ipv4_addr&);
Error from_view(View&, Ipv6_addr&);


/// The OpenFlow header is found at the front of every OpenFlow message.
struct Header {
  static constexpr std::size_t bytes = 8;

  Uint8 version;
  Uint8 type;
  Uint16 length;
  Uint32 xid;
};

Error to_view(View&, const Header&);
Error from_view(View&, Header&);

} // namespace ofp
} // freeflow

#include <freeflow/proto/ofp/ofp.ipp>

#endif
