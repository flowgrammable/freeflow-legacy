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

#ifndef FREEFLOW_OFPV1_0_STATS_HPP
#define FREEFLOW_OFPV1_0_STATS_HPP

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/ofp.hpp>
#include <freeflow/proto/ofp/v1.0/error.hpp>
#include <freeflow/proto/ofp/v1.0/port.hpp>
#include <freeflow/proto/ofp/v1.0/match.hpp>
#include <freeflow/proto/ofp/v1.0/action.hpp>

namespace freeflow {
namespace ofp {
namespace v1_0 {

/// Enumerates the different types of stats requests and replies.
///
/// Note that the STATS_INVALID type is used internally to describe
/// the uninitialized state of non-trivial stats unions.
enum Stats_type : Uint16 {
  STATS_DESC      = 0x00,
  STATS_FLOW      = 0x01,
  STATS_AGGREGATE = 0x02,
  STATS_TABLE     = 0x03,
  STATS_PORT      = 0x04,
  STATS_QUEUE     = 0x05,
  STATS_VENDOR    = 0xffff,
  STATS_INVALID   = 0x7fff
};

/// Represents a request for statistics or information that does
/// not contain any additional information. This includes
/// description information and table stats.
struct Empty_stats_request { };

/// Represents a request for flow and aggregate flow statistics.
struct Flow_stats_request {
  Match    match;
  Uint8    table_id;
  Port::Id out_port;
};

/// Represents a request for port information. Note that the port number
/// is the numer
struct Port_stats_request {
  Port::Id port_number;
};

/// Represents a request for queue information.
struct Queue_stats_request {
  Port::Id port_number;
  Uint16   queue_id;
};

/// Represents a reuest for vendor information.
struct Vendor_stats_request {
  Uint32 vendor_id;
};

struct Description_stats {
  String<256> mfr_desc;
  String<256> hw_desc;
  String<256> sw_desc;
  String<32>  serial_number;
  String<256> dp_desc;
};

struct Flow_stats_entry {
  Uint16      length;
  Uint8       table_id;
  Match       match;
  Uint32      duration_sec;
  Uint32      duration_nsec;
  Uint16      priority;
  Uint16      idle_timeout;
  Uint16      hard_timeout;
  Uint64      cookie;
  Uint64      packet_count;
  Uint64      byte_count;
  Action_list actions;
};

using Flow_stats = Sequence<Flow_stats_entry>;

struct Aggregate_stats {
  Uint64      packet_count;
  Uint64      byte_count;
  Uint32      flow_count;
};

struct Table_stats_entry {
  Uint8 table_id;
  String<128> name;
  Uint32 wildcards;
  Uint32 max_entries;
  Uint32 active_count;
  Uint64 lookup_count;
  Uint64 matched_count;
};

using Table_stats = Sequence<Table_stats_entry>;

struct Port_stats_entry {
  Port::Id port_number;
  Uint64 rx_packets;
  Uint64 tx_packets;
  Uint64 rx_bytes;
  Uint64 tx_bytes;
  Uint64 rx_dropped;
  Uint64 tx_dropped;
  Uint64 rx_errors;
  Uint64 tx_errors;
  Uint64 rx_frame_err;
  Uint64 rx_over_err;
  Uint64 rx_crc_err;
  Uint64 collisions;
};

using Port_stats = Sequence<Port_stats_entry>;

struct Queue_stats_entry {
  Uint16 length;
  Uint32 queue_id;
  Uint64 tx_bytes;
  Uint64 tx_packets;
  Uint64 tx_errors;
};

using Queue_stats = Sequence<Queue_stats_entry>;

struct Vendor_stats {
  Uint32 vendor_id;
  Buffer data;
};

/// The payload of a stats request.
union Stats_request_payload {
  Empty_stats_request  empty;
  Flow_stats_request   flow;
  Port_stats_request   port;
  Queue_stats_request  queue;
  Vendor_stats_request vendor;
};

/// The payload of a stats reply.
union Stats_reply_payload {
  // This union contains non-trivial members.
  Stats_reply_payload() { }
  ~Stats_reply_payload() { }

  Description_stats desc;
  Flow_stats        flow;
  Aggregate_stats   aggr;
  Table_stats       table;
  Port_stats        port;
  Queue_stats       queue;
  Vendor_stats      vendor;
};

/// Represents the header of a stats request or response message.
struct Stats_header {
  enum Flags : Uint16 { };

  Stats_header() = default;
  Stats_header(Stats_type t);

  Stats_type type;
  Flags flags;
};

// Operations
void construct(Stats_reply_payload&, Stats_type);
void destroy(Stats_reply_payload&, Stats_type);

// Protocol

using ofp::bytes;
using ofp::to_view;
using ofp::from_view;

// Bytes
constexpr std::size_t bytes(const Empty_stats_request&);
constexpr std::size_t bytes(const Flow_stats_request&);
constexpr std::size_t bytes(const Port_stats_request&);
constexpr std::size_t bytes(const Queue_stats_request&);
constexpr std::size_t bytes(const Vendor_stats_request&);
constexpr std::size_t bytes(const Description_stats&);
std::size_t bytes(const Flow_stats_entry&);
constexpr std::size_t bytes(const Aggregate_stats&);
constexpr std::size_t bytes(const Table_stats_entry&);
constexpr std::size_t bytes(const Port_stats_entry&);
constexpr std::size_t bytes(const Queue_stats_entry&);
std::size_t bytes(const Vendor_stats&);
constexpr std::size_t bytes(const Stats_header&);
std::size_t bytes(const Stats_request_payload&, Stats_type);
std::size_t bytes(const Stats_reply_payload&, Stats_type);

// To view
Error to_view(View&, const Empty_stats_request&);
Error to_view(View&, const Flow_stats_request&);
Error to_view(View&, const Port_stats_request&);
Error to_view(View&, const Queue_stats_request&);
Error to_view(View&, const Vendor_stats_request&);
Error to_view(View&, const Description_stats&);
Error to_view(View&, const Flow_stats_entry&);
Error to_view(View&, const Aggregate_stats&);
Error to_view(View&, const Table_stats_entry&);
Error to_view(View&, const Port_stats_entry&);
Error to_view(View&, const Queue_stats_entry&);
Error to_view(View&, const Vendor_stats&);
Error to_view(View&, const Stats_header&);
Error to_view(View&, const Stats_request_payload&, Stats_type);
Error to_view(View&, const Stats_reply_payload&, Stats_type);

// From view
Error from_view(View&, Empty_stats_request&);
Error from_view(View&, Flow_stats_request&);
Error from_view(View&, Port_stats_request&);
Error from_view(View&, Queue_stats_request&);
Error from_view(View&, Vendor_stats_request&);
Error from_view(View&, Description_stats&);
Error from_view(View&, Flow_stats_entry&);
Error from_view(View&, Aggregate_stats&);
Error from_view(View&, Table_stats_entry&);
Error from_view(View&, Port_stats_entry&);
Error from_view(View&, Queue_stats_entry&);
Error from_view(View&, Vendor_stats&);
Error from_view(View&, Stats_header&);
Error from_view(View&, Stats_request_payload&, Stats_type);
Error from_view(View&, Stats_reply_payload&, Stats_type);

// Validation
constexpr bool is_valid(Stats_type);

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#include "stats.ipp"

#endif
