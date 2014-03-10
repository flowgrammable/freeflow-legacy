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

#ifndef FREEFLOW_OFPV1_0_HPP
#define FREEFLOW_OFPV1_0_HPP

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/ofp.hpp>
#include <freeflow/proto/ofp/v1.0/error.hpp>
#include <freeflow/proto/ofp/v1.0/port.hpp>
#include <freeflow/proto/ofp/v1.0/match.hpp>

namespace freeflow {
namespace ofp {
namespace v1_0 {

/// The type of supported messages.
enum Message_type : Uint16 {
  HELLO                    = 0,
  ERROR                    = 1,
  ECHO_REQUEST             = 2,
  ECHO_REPLY               = 3,
  VENDOR                   = 4,
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
  PORT_MOD                 = 15,
  STATS_REQUEST            = 16,
  STATS_REPLY              = 17,
  BARRIER_REQUEST          = 18,
  BARRIER_REPLY            = 19,
  QUEUE_GET_CONFIG_REQUEST = 20,
  QUEUE_GET_CONFIG_REPLY   = 21
};

/// Represents an empty message component. No additional data is provided
/// beyond a header.
struct Empty { };

/// The Hello message is sent to negotiate the protocol version.
///
/// TODO: Does this message need to be made version neutral?
struct Hello {
  Buffer data;
};

/// The Error message is sent to indicate an error in a previous
/// communication.
struct Error {
  Uint16 type;
  Uint16 code;
  Buffer data;
};

/// The Echo message is used to communicate both echo requests and
/// replys between the switch and the controller.
struct Echo {
  Buffer data;
};

struct Vendor {
  Uint32 vendor_id;
  Buffer data;
};

struct Feature {
  enum Capability : Uint32 {
    FLOW_STATS   = 0x00000001,
    TABLE_STATS  = 0x00000002,
    PORT_STATS   = 0x00000004,
    STP          = 0x00000008,
    RESERVED     = 0x00000010,
    IP_REASM     = 0x00000020,
    QUEUE_STATS  = 0x00000040,
    ARP_MATCH_IP = 0x00000080 
  };

  enum Action : Uint32 {
    OUTPUT       = 0x00000001,
    SET_VLAN_VID = 0x00000002,
    SET_VLAN_PCP = 0x00000003,
    STRIP_VLAN   = 0x00000008,
    SET_DL_SRC   = 0x00000010,
    SET_DL_DST   = 0x00000020,
    SET_NW_SRC   = 0x00000040,
    SET_NW_DST   = 0x00000080,
    SET_NW_TOS   = 0x00000100,
    SET_TP_SRC   = 0x00000200,
    SET_TP_DST   = 0x00000400,
    ENQUEUE      = 0x00000800 
  };

  Uint64         datapath_id;
  Uint32         nbuffers;
  Uint8          ntables;
  Capability     capabilities;
  Action         actions;
  Sequence<Port> ports;
};

/// A Config message is sent as a response to a configuration request,
/// or to modify the configuration of the switch.
struct Config {
  enum Flags : Uint16 {
    FRAG_NORMAL = 0x0000, 
    FRAG_DROP   = 0x0001, 
    FRAG_REASM  = 0x0002, 
    FRAG_MASK   = 0x0003
  };

  Flags  flags;
  Uint16 miss_send_len;
};

/// A Packet_in message is sent from the switch to the controller to
/// indicate a miss in the flow tables or because a specific action
/// has been requested.
struct Packet_in {
  enum Reason : Uint8 {
    NO_MATCH = 0x00,
    ACTION = 0x01
  };

  Uint32   buffer_id;
  Uint16   total_len;
  Port::Id in_port;
  Reason   reason;
  Buffer   data;
};

/// A Flow_removed message is sent from the switch to the controller to
/// indicate that an entry has been removed from a flow table.
struct Flow_removed {
  enum Reason : Uint8 {
    IDLE_TIMEOUT = 0x00,
    HARD_TIMEOUT = 0x01,
    DELETE       = 0x02,        
  };

  Match  match;
  Uint64 cookie;
  Uint16 priority;
  Reason reason;
  Uint32 duration_sec;
  Uint32 duration_nsec;
  Uint16 idle_timeout;
  Uint64 packet_count;
  Uint64 byte_count;
};


/// The message class embodies a specific kind of OpenFlow message.
struct Message {
  using Type = Message_type;
  union Payload {
    Payload() { }
    ~Payload() { }
    
    Empty        empty;
    Hello        hello;
    Error        error;
    Echo         echo;
    Vendor       vendor;
    Feature      feature;
    Config       config;
    Packet_in    packet_in;
    Flow_removed flow_removed;
  };

  Message(Type);
  ~Message();

  Type    type;
  Payload payload;
};

// Protocol

using ofp::to_view;
using ofp::from_view;
using ofp::bytes;

constexpr std::size_t bytes(const Empty&);
std::size_t bytes(const Hello&);
std::size_t bytes(const Error&);
std::size_t bytes(const Echo&);
std::size_t bytes(const Vendor&);
std::size_t bytes(const Feature&);
constexpr std::size_t bytes(const Config&);
std::size_t bytes(const Packet_in&);
constexpr std::size_t bytes(const Flow_removed&);
std::size_t bytes(const Message&);

Errc to_view(View&, const Empty&);
Errc to_view(View&, const Hello&);
Errc to_view(View&, const Error&);
Errc to_view(View&, const Echo&);
Errc to_view(View&, const Vendor&);
Errc to_view(View&, const Feature&);
Errc to_view(View&, const Config&);
Errc to_view(View&, const Packet_in&);
Errc to_view(View&, const Flow_removed&);
Errc to_view(View&, const Message& m);

Errc from_view(View&, Empty&);
Errc from_view(View&, Hello&);
Errc from_view(View&, Error&);
Errc from_view(View&, Echo&);
Errc from_view(View&, Vendor&);
Errc from_view(View&, Feature&);
Errc from_view(View&, Config&);
Errc from_view(View&, Packet_in&);
Errc from_view(View&, Flow_removed&);
Errc from_view(View&, Message& m);

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#include "message.ipp"

#endif
