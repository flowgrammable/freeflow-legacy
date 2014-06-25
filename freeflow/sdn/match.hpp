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

#ifndef FREEFLOW_MATCH_HPP
#define FREEFLOW_MATCH_HPP

namespace freeflow {

struct Match {
  bool in_port;            // True
  bool in_port_mask;       // From StatsRes.Table.wildcards
  bool eth_src;            // True
  bool eth_src_mask;       // From StatsRes.Table.wildcards
  bool eth_dst;            // True
  bool eth_dst_mask;       // From StatsRes.Table.wildcards
  bool vlan_id;            // True
  bool vlan_id_mask;       // From StatsRes.Table.wildcards
  bool vlan_pcp;           // True
  bool vlan_pcp_mask;      // From StatsRes.Table.wildcards
  bool eth_type;           // True
  bool eth_type_mask;      // From StatsRes.Table.wildcards
  bool arp_opcode;         // From FeatureRes.capabilities
  bool arp_opcode_mask;    // From FeatureRes.capabilities
  bool arp_spa;            // From FeatureRes.capabilities
  bool arp_spa_mask;       // From FeatureRes.capabilities
  bool arp_tpa;            // From FeatureRes.capabilities
  bool arp_tpa_mask;       // From FeatureRes.capabilities
  bool ipv4_tos;           // True
  bool ipv4_tos_mask;      // From FeatureRes.capabilities
  bool ipv4_protocol;      // True
  bool ipv4_protocol_mask; // From FeatureRes.capabilities
  bool ipv4_src;           // True
  bool ipv4_src_mask;      // From FeatureRes.capabilities
  bool ipv4_dst;           // True
  bool ipv4_dst_mask;      // From FeatureRes.capabilities
  bool tcp_src;            // True
  bool tcp_src_mask;       // From FeatureRes.capabilities
  bool tcp_dst;            // True
  bool tcp_dst_mask;       // From FeatureRes.capabilities
  bool udp_src;            // True
  bool udp_src_mask;       // From FeatureRes.capabilities
  bool udp_dst;            // True
  bool udp_dst_mask;       // From FeatureRes.capabilities
  bool icmpv4_type;        // True
  bool icmpv4_type_mask;   // From FeatureRes.capabilities
  bool icmpv4_code;        // True
  bool icmpv4_code_mask;   // From FeatureRes.capabilities
};

} // namespace freeflow

#include <freeflow/sdn/match.ipp>

#endif