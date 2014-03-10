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

#ifndef FREEFLOW_OFPV1_0_MATCH_HPP
#define FREEFLOW_OFPV1_0_MATCH_HPP

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/ofp.hpp>
#include <freeflow/proto/ofp/v1.0/error.hpp>

namespace freeflow {
namespace ofp {
namespace v1_0 {

/// The Match sturcture defines a classifier over packets.
struct Match {
  /// A Match's wildcards indicate fields of different protocol levels
  /// that can be matched in a flow table.
  enum Wildcards : uint32_t
  {
    IN_PORT     = 0x00000001, ///< Any switch port
    DL_VLAN     = 0x00000002, ///< Any VLAN id
    DL_SRC      = 0x00000004, ///< Any Ethernet source address
    DL_DST      = 0x00000008, ///< Any Ethernet destination address
    DL_TYPE     = 0x00000010, ///< Any Ethernet frame type
    NW_PROTO    = 0x00000020, ///< Any IP protocol
    TP_SRC      = 0x00000040, ///< Any IP source port
    TP_DST      = 0x00000080, ///< Any IP target port
    NW_SRC      = 0x00003f00,
    NW_DST      = 0x000fb000,
    DL_VLAN_PCP = 0x00100000, ///< Any VLAN priority 
    NW_TOS      = 0x00200000, ///< Any IP ToS
    ALL         = 0x003fffff  ///< Anything
  };

  Wildcards wildcards;
  Uint16    in_port;
  Mac_addr  dl_src;
  Mac_addr  dl_dst;
  Uint16    dl_vlan;
  Uint8     dl_pcp;
  Uint16    dl_type;
  Uint8     nw_tos;
  Uint8     nw_proto;
  Ipv4_addr nw_src;
  Ipv4_addr nw_dst;
  Uint16    tp_src;
  Uint16    tp_dst;
};

// Protocol
constexpr std::size_t bytes(const Match&);
Errc to_view(View&, const Match&);
Errc from_view(View&, Match&);

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#include "match.ipp"

#endif