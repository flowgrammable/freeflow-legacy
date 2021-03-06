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
#include <freeflow/proto/ofp/v1.0/error.hpp>
#include <freeflow/proto/ofp/v1.0/port.hpp>

namespace freeflow {
namespace ofp {
namespace v1_0 {

/// The types of actions supported by the protocol
enum Action_type : Uint16 {
  ACTION_OUTPUT       = 0x00, 
  ACTION_SET_VLAN_VID = 0x01, 
  ACTION_SET_VLAN_PCP = 0x02, 
  ACTION_STRIP_VLAN   = 0x03, 
  ACTION_SET_DL_SRC   = 0x04, 
  ACTION_SET_DL_DST   = 0x05,
  ACTION_SET_NW_SRC   = 0x06, 
  ACTION_SET_NW_DST   = 0x07, 
  ACTION_SET_NW_TOS   = 0x08,
  ACTION_SET_TP_SRC   = 0x09, 
  ACTION_SET_TP_DST   = 0x0a, 
  ACTION_ENQUEUE      = 0x0b,
  ACTION_VENDOR       = 0xffff
};

/// The empty action represents an action that requires no additional
/// data.
struct Action_empty { };

/// Requests packet output. The action requires the switch to send a 
/// maximum number of bytes to a specified port.
struct Action_output {
  Port::Id    port;
  Uint16      max_len;
};

/// Forwards a packet through a queue on a specified port.
struct Action_enqueue {
  Uint16 port;
  Uint32 queue;
};

/// Sets a packet's VLAN VID to the given value. The value 0xffff indicates
/// that no VLAN VID is set.
struct Action_vlan_vid {
  Uint16 value;
};

/// Sets a packet's VLAN PCP to the given value.
struct Action_vlan_pcp {
  Uint8 value;
};

/// Sets a packet's source or target MAC address.
struct Action_dl_addr {
  Mac_addr addr;
};

// Sets a packet's source or target IP address.
struct Action_nw_addr {
  Ipv4_addr addr;
};

// Sets a packet's ToS.
struct Action_nw_tos {
  Uint8 value;
};

/// Sets a packet's source or target port number. 
///
/// Note that the notion of "port" depends on the transport protocol. 
/// For TCP and UDP, the port is a 16-bit port number. For ICMP, the 
/// lower 8 bits can specify an ICMP code.
struct Action_tp_port {
  Uint16 port;
};

struct Action_vendor {
  Uint32 vendor;
};


/// The Action type represents is a structure common to all action types.
/// It provides the type and length of the header.
///
/// Note that OpenFlow specification includes the header as part of all
/// action types. The implementation factors the header out, meaning each
/// specific action contains only the fields specific to that action.
struct Action_header {
  Action_type type;
  Uint16      length;
};

/// The action payload contains the different kinds of action payload.
union Action_payload {
  Action_empty    empty;
  Action_output   output;
  Action_enqueue  enqueue;
  Action_vlan_vid vlan_vid;
  Action_vlan_pcp vlan_pcp;
  Action_dl_addr  dl_addr;
  Action_nw_addr  nw_addr;
  Action_nw_tos   nw_tos;
  Action_tp_port  tp_port;
  Action_vendor   vendor;
};

/// The Action class contains an action header and the corresponding
/// payload.
struct Action {
  Action_header  header;
  Action_payload payload;
};

/// A sequence of actions
using Action_list = Sequence<Action>;


// Operations
std::size_t payload_bytes(const Action_header&);
std::size_t payload_bytes(const Action&);

// Protocol
constexpr std::size_t bytes(const Action_empty&);
constexpr std::size_t bytes(const Action_output&);
constexpr std::size_t bytes(const Action_enqueue&);
constexpr std::size_t bytes(const Action_vlan_vid&);
constexpr std::size_t bytes(const Action_vlan_pcp&);
constexpr std::size_t bytes(const Action_dl_addr&);
constexpr std::size_t bytes(const Action_nw_addr&);
constexpr std::size_t bytes(const Action_nw_tos&);
constexpr std::size_t bytes(const Action_tp_port&);
constexpr std::size_t bytes(const Action_vendor&);
constexpr std::size_t bytes(const Action_header&);
std::size_t bytes(const Action_payload&, Action_type);
std::size_t bytes(const Action&);

Error to_view(View&, const Action_empty&);
Error to_view(View&, const Action_output&);
Error to_view(View&, const Action_enqueue&);
Error to_view(View&, const Action_vlan_vid&);
Error to_view(View&, const Action_vlan_pcp&);
Error to_view(View&, const Action_dl_addr&);
Error to_view(View&, const Action_nw_addr&);
Error to_view(View&, const Action_nw_tos&);
Error to_view(View&, const Action_tp_port&);
Error to_view(View&, const Action_vendor&);
Error to_view(View&, const Action_payload&, Action_type);
Error to_view(View&, const Action&);

Error from_view(View&, Action_empty&);
Error from_view(View&, Action_output&);
Error from_view(View&, Action_enqueue&);
Error from_view(View&, Action_vlan_vid&);
Error from_view(View&, Action_vlan_pcp&);
Error from_view(View&, Action_dl_addr&);
Error from_view(View&, Action_nw_addr&);
Error from_view(View&, Action_nw_tos&);
Error from_view(View&, Action_tp_port&);
Error from_view(View&, Action_vendor&);
Error from_view(View&, Action_payload&, Action_type);
Error from_view(View&, Action&);

bool is_valid(Action_type);

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#include "action.ipp"

#endif
