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

#ifndef FREEFLOW_OFPV1_3_0_HPP
#define FREEFLOW_OFPV1_3_0_HPP

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/ofp.hpp>
#include <freeflow/proto/ofp/protocol.hpp>

namespace freeflow {
namespace ofp {
namespace v1_3_0 {

/// The version type.
enum Version_type : Uint8 { VERSION = 4 };

/// The types of supported messages.
enum Message_type : Uint8 {
  HELLO                    = 0,
  ERROR                    = 1,
  ECHO_REQUEST             = 2,
  ECHO_REPLY               = 3,
  EXPERIMENTER             = 4,
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
  GROUP_MOD                = 15,
  PORT_MOD                 = 16,
  TABLE_MOD                = 17,
  MULTIPART_REQUEST        = 18,
  MULTIPART_REPLY          = 19,
  BARRIER_REQUEST          = 20,
  BARRIER_REPLY            = 21,
  QUEUE_GET_CONFIG_REQUEST = 22,
  QUEUE_GET_CONFIG_REPLY   = 23,
  ROLE_REQUEST             = 24,
  ROLE_REPLY               = 25,
  GET_ASYNC_REQUEST        = 26,
  GET_ASYNC_REPLY          = 27,
  SET_ASYNC                = 28,
  METER_MOD                = 29
};

// -------------------------------------------------------------------------- //
// Messages

/// The Hello message is sent to negotiate the protocol version.
///
/// TODO: Does this message need to be made version neutral?
struct Hello {
  static constexpr Message_type Kind = HELLO;
  
  Buffer data;
};

// -------------------------------------------------------------------------- //
// Operations
// --construct/destroy functions here?--

// Protocol

using ofp::to_view;
using ofp::from_view;
using ofp::bytes;

// -------------------------------------------------------------------------- //
// Bytes
// --bytes functions here--

// -------------------------------------------------------------------------- //
// To View
// --to_view functions here--

// -------------------------------------------------------------------------- //
// From View
// --from_view functions here--

// -------------------------------------------------------------------------- //
// Validation
// --is_valid functions here--

} // namespace v1_3_0
} // namespace ofp
} // namespace freeflow

#include "message.ipp"

#endif
