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

namespace freeflow {
namespace ofp {
namespace v1_0 {

constexpr std::size_t 
bytes(const Action_empty&) { return 0; }

constexpr std::size_t 
bytes(const Action_output&) { return 4; }

constexpr std::size_t 
bytes(const Action_enqueue&) { return 8; }

constexpr std::size_t 
bytes(const Action_vlan_vid&) { return 4; }

constexpr std::size_t 
bytes(const Action_vlan_pcp&) { return 4; }

constexpr std::size_t 
bytes(const Action_dl_addr&) { return 8; }

constexpr std::size_t 
bytes(const Action_nw_addr&) { return 4; }

constexpr std::size_t 
bytes(const Action_nw_tos&) { return 4; }

constexpr std::size_t 
bytes(const Action_tp_port&) { return 4; }

constexpr std::size_t 
bytes(const Action_vendor&) { return 4; }


} // namespace v1_0
} // namespace ofp
} // namespace freeflow