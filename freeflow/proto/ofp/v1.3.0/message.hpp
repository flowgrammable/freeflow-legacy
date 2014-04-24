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
  HELLO                = 0, ///< see Hello
  ERROR                = 1, ///< see Error
  ECHO_REQ             = 2, ///< see Echo_req
  ECHO_RES             = 3, ///< see Echo_res
  EXPERIMENTER         = 4, ///< see Experimenter
  FEATURE_REQ          = 5, ///< see Feature_req
  FEATURE_RES          = 6, ///< see Feature_res
  GET_CONFIG_REQ       = 7, ///< see Get_config_req
  GET_CONFIG_RES       = 8, ///< see Get_config_res
  SET_CONFIG           = 9, ///< see Set_config
  PACKET_IN            = 10, ///< see Packet_in
  FLOW_REMOVED         = 11, ///< see Flow_removed
  PORT_STATUS          = 12, ///< see Port_status
  PACKET_OUT           = 13, ///< see Packet_out
  FLOW_MOD             = 14, ///< see Flow_mod
  GROUP_MOD            = 15, ///< see Group_mod
  PORT_MOD             = 16, ///< see Port_mod
  TABLE_MOD            = 17, ///< see Table_mod
  MULTIPART_REQ        = 18, ///< see Multipart_req
  MULTIPART_RES        = 19, ///< see Multipart_res
  BARRIER_REQ          = 20, ///< see Barrier_req
  BARRIER_RES          = 21, ///< see Barrier_res
  QUEUE_GET_CONFIG_REQ = 22, ///< see Queue_get_config_req
  QUEUE_GET_CONFIG_RES = 23, ///< see Queue_get_config_res
  ROLE_REQ             = 24, ///< see Role_req
  ROLE_RES             = 25, ///< see Role_res
  GET_ASYNC_REQ        = 26, ///< see Get_async_req
  GET_ASYNC_RES        = 27, ///< see Get_async_res
  SET_ASYNC            = 28, ///< see Set_async
  METER_MOD            = 29  ///< see Meter_mod
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
