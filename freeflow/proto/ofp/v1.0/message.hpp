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

#ifndef FREEFLOW_OFPV1_0_HPP
#define FREEFLOW_OFPV1_0_HPP

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/ofp.hpp>

#include "port.hpp"

namespace freeflow {
namespace ofp {
namespace v1_0 {

// Note that this class is named "Errc" (for error code) in this 
// namespace to avoid collisions with the Error message.
struct Errc : ofp::Error {
  /// The message type is unsupported
  static constexpr Code BAD_MESSAGE_TYPE = 200;

  static constexpr Code HELLO_OVERFLOW = 201;
  static constexpr Code ERROR_OVERFLOW = 201;

  using ofp::Error::Error;
};

/// The type of supported messages.
enum Message_type : Uint16 {
  HELLO                    = 0,
  ERROR                    = 1,
  ECHO_REQUEST             = 2,
  ECHO_REPLY               = 3,
  VENDOR                   = 4,
  FEATURE_REQUEST          = 5,
  FEATURE_REPLY            = 6,
  GET_CONFIG_REQUEST       = 7,
  GET_CONFIG_REPLY         = 8,
  SET_CONFIG               = 9,
  PACKET_IN                = 10,
  FLOW_REMOVED             = 11,
  PORT_STATUS              = 12,
  PACKET_OUT               = 13,
  FLOW_MOD                 = 14,
  PORT_MOD                 = 15,
  STATS_REQUEST            = 16,
  STATS_REPLY              = 17,
  BARRIER_REQUEST          = 18,
  BARRIER_REPLY            = 19,
  QUEUE_GET_CONFIG_REQUEST = 20,
  QUEUE_GET_CONFIG_REPLY   = 21
};

struct Hello {
  static constexpr std::size_t bytes = 0;
  Buffer data;
};

struct Error {
  static constexpr std::size_t bytes = 4;
  Uint16 type;
  Uint16 code;
  Buffer data;
};

/// The Echo message is used to communicate both echo requests and
/// replys between the switch and the controller.
struct Echo {
  static constexpr std::size_t bytes = 0;
  Buffer data;
};


struct Message {
  using Type = Message_type;
  union Payload {
    Payload() { }
    ~Payload() { }
    
    Hello hello;
    Error error;
    Echo  echo;
  };

  Message(Type);
  ~Message();

  Type    type;
  Payload payload;
};

std::size_t bytes(const Hello&);
std::size_t bytes(const Error&);
std::size_t bytes(const Echo&);
std::size_t bytes(const Message&);

Errc to_view(View&, const Hello&);
Errc to_view(View&, const Error&);
Errc to_view(View&, const Echo&);
Errc to_view(View&, const Message& m);

Errc from_view(View&, Hello&);
Errc from_view(View&, Error&);
Errc from_view(View&, Echo&);
Errc from_view(View&, Message& m);


} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#include "message.ipp"

#endif
