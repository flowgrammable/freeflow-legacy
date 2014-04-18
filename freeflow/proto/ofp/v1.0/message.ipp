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

// -------------------------------------------------------------------------- //
// Messages

inline
Stats_reply::Stats_reply()
  : header(STATS_INVALID)
{ }

inline
Stats_reply::~Stats_reply() {
  destroy(payload, header.type);
}

inline
Header::Header(Message_type t, Uint16 n, Uint32 x)
  : version(VERSION), type(t), length(n), xid(x) { }


// Bytes

inline std::size_t 
bytes(const Hello& m) { return m.data.size(); }

inline std::size_t 
bytes(const Error& m) { return 4 + m.data.size(); }

inline std::size_t 
bytes(const Echo& m) { return m.data.size(); }

inline std::size_t 
bytes(const Vendor& m) { return 4 + m.data.size(); }

inline std::size_t 
bytes(const Feature_request& m) { return 0; }

inline std::size_t 
bytes(const Feature_reply& m) { return 24 + m.ports.size(); }

constexpr std::size_t 
bytes(const Config&) { return 4; }

inline std::size_t
bytes(const Packet_in& m) { return 10 + m.data.size(); }

constexpr std::size_t
bytes(const Flow_removed& m) { return 40 + bytes(m.match); }

constexpr std::size_t
bytes(const Port_status& m) { return 8 + bytes(m.port); }

inline std::size_t
bytes(const Packet_out& m) { return 8 + bytes(m.actions) + bytes(m.data); }

inline std::size_t
bytes(const Flow_mod& m) { return 24 + bytes(m.match) + bytes(m.actions); }

constexpr std::size_t
bytes(const Port_mod& m) { return 24; }

inline std::size_t
bytes(const Stats_request& m) { 
  return bytes(m.header) + bytes(m.payload, m.header.type); 
}

inline std::size_t
bytes(const Stats_reply& m) { 
  return bytes(m.header) + bytes(m.payload, m.header.type); 
}

inline std::size_t
bytes(const Queue_config_request& m) { return 2; }

inline std::size_t
bytes(const Queue_config_reply& m) { return 8 + bytes(m.queues); }

constexpr std::size_t
bytes(const Header& m) { return 8; }

inline std::size_t
bytes(const Message& m) { 
  return bytes(m.header) + bytes(m.payload, Message_type(m.header.type)); 
}

// Validation

constexpr bool
is_valid(Version_type t) { return t == 1; }

constexpr bool
is_valid(Message_type t) { return 0 <= t and t <= QUEUE_GET_CONFIG_REPLY; }

} // namespace v1_0
} // namespace ofp
} // namespace freeflow
