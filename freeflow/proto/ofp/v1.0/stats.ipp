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

// Structures

inline
Stats_header::Stats_header(Stats_type t)
  : type(t), flags() 
{ }

// Bytes
constexpr std::size_t 
bytes(const Empty_stats_request&) { return 0; }

constexpr std::size_t 
bytes(const Flow_stats_request& m) { return bytes(m.match) + 4; }

constexpr std::size_t 
bytes(const Port_stats_request&) { return 8; }

constexpr std::size_t 
bytes(const Queue_stats_request&) { return 8; }

constexpr std::size_t 
bytes(const Vendor_stats_request&) { return 4; }

constexpr std::size_t 
bytes(const Description_stats&) { return 1056; }

inline std::size_t 
bytes(const Flow_stats_entry& m) { return 44 + bytes(m.match) + bytes(m.actions); }

constexpr std::size_t 
bytes(const Aggregate_stats&) { return 24; }

constexpr std::size_t 
bytes(const Table_stats_entry&) { return 64; }

constexpr std::size_t 
bytes(const Port_stats_entry&) { return 104; }

constexpr std::size_t 
bytes(const Queue_stats_entry&) { return 32; }

inline std::size_t 
bytes(const Vendor_stats& m) { return 4 + bytes(m.data); }

constexpr std::size_t 
bytes(const Stats_header& m) { return 4; }

// To view

inline Error 
to_view(View&, const Empty_stats_request&) { return {}; }

// From view

inline Error 
from_view(View&, Empty_stats_request&) { return {}; }

// Validation

constexpr bool
is_valid(Stats_type t) {
  return (0x00 <= t and t <= 0x05) or t == 0xffff;
}

} // namespace v1_0
} // namespace ofp
} // namespace freeflow