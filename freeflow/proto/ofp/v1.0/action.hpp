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

#ifndef FREEFLOW_OFPV1_0_ACTION_HPP
#define FREEFLOW_OFPV1_0_ACTION_HPP

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/ofp.hpp>
#include <freeflow/proto/ofp/v1.0/port.hpp>

namespace freeflow {
namespace ofp {
namespace v1_0 {

/// The types of actions supported by the protocol
enum Action_type : Uint16 {
  ACTION_OUTPUT       = 0x0000, 
  ACTION_SET_VLAN_VID = 0x0001, 
  ACTION_SET_VLAN_PCP = 0x0002, 
  ACTION_STRIP_VLAN   = 0x0003, 
  ACTION_SET_DL_SRC   = 0x0004, 
  ACTION_SET_DL_DST   = 0x0005,
  ACTION_SET_NW_SRC   = 0x0006, 
  ACTION_SET_NW_DST   = 0x0007, 
  ACTION_SET_NW_TOS   = 0x0008,
  ACTION_SET_TP_SRC   = 0x0009, 
  ACTION_SET_TP_DST   = 0x000a, 
  ACTION_ENQUEUE      = 0x000b,
  ACTION_VENDOR       = 0xffff
};

/// The Action type represents is a structure common to all action types.
/// It provides the type and length of the header.
///
/// Note that OpenFlow specification includes the header as part of all
/// action types. The implementation factors the header out, meaning each
/// specific action contains only the fields specific to that action.
struct Action_header {
  static constexpr std::size_t bytes = 4;
  
  Action_type type;
  Uint16      length;
};

/// Requests packet output. The action requires the switch to send a 
/// maximum number of bytes to a specified port.
struct Action_output {
  static constexpr std::size_t bytes = 4;

  Port::Id    port;
  Uint16      max_len;
};

/// Forwards a packet through a queue on a specified port.
struct Action_enqueue {
  static constexpr std::size_t bytes = 8;
  
  Uint16 port;
  Uint32 queue_id;
};

/// Sets a packet's VLAN VID to the given value. The value 0xffff indicates
/// that no VLAN VID is set.
struct Action_set_vlan_vid {
  static constexpr std::size_t bytes = 4;

  Uint16 vlan_vid;
};

/// Sets a packet's VLAN PCP to the given value.
struct Action_set_vlan_pcp {
  static constexpr std::size_t bytes = 4;

  Uint8 value;
};

/// Removes a packet's VLAN tag.
struct Action_strip_vlan {
  static constexpr std::size_t bytes = 0;
};

/// Sets a packet's source or target MAC address.
struct Action_set_dl_src {
  static constexpr std::size_t bytes = 8;
  Mac_addr dl_addr;
};

// Sets a packet's source or target IP address.
struct Action_set_nw_src {
  static constexpr std::size_t bytes = 4;
  Ipv4_addr addr;
};

// Sets a packet's ToS.
struct Action_set_nw_tos {
  static constexpr std::size_t bytes = 4;
  Uint8 value;
};

/// Sets a packet's source or target port number. 
///
/// Note that the notion of "port" depends on the transport protocol. 
/// For TCP and UDP, the port is a 16-bit port number. For ICMP, the 
/// lower 8 bits can specify an ICMP code.
struct Action_set_tp_port {
  Uint16 value;
};

struct Action_vendor {
  Uint32 vendor;
};

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#endif
