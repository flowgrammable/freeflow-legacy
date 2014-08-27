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
#include <freeflow/proto/ofp/v1.0/queue.hpp>
#include <freeflow/proto/ofp/v1.0/match.hpp>
#include <freeflow/proto/ofp/v1.0/action.hpp>
#include <freeflow/proto/ofp/v1.0/stats.hpp>

namespace freeflow {
namespace ofp {
namespace v1_0 {

/// The version type.
enum Version_type : Uint8 { VERSION = 1 };

/// The type of supported messages.
enum Message_type : Uint8 {
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

/// The Hello message is sent to negotiate the protocol version.
///
/// TODO: Does this message need to be made version neutral?
struct Hello {
  static constexpr Message_type Kind = HELLO;
  
  Buffer data;
};

/// The Error message is sent to indicate an error in a previous
/// communication.
///
/// \todo Integrate error codes with errc. These should be composed
/// by shifting the entire code into a 32-bit space s.t. the type is
/// represents the high-order bits and the code the low-order bits.
struct Error_message {
  static constexpr Message_type Kind = ERROR;

  enum Type {
    HELLO_FAILED,
    BAD_REQUEST,
    BAD_ACTION,
    FLOW_MOD_FAILED,
    PORT_MOD_FAILED,
    QUEUE_OP_FAILED 
  };

  enum Hello_failed {
    HF_INCOMPATIBLE = 0, ///< Incompatible version
    HF_EPERM        = 1  ///< Permission error
  };

  enum Bad_request {
    BR_BAD_VERSION    = 0, ///< Header version not supprted
    BR_BAD_TYPE       = 1, ///< Message type not supported
    BR_BAD_STAT       = 2, ///< Stat request type not supported
    BR_BAD_VENDOR     = 3, ///< Vendor not supported
    BR_BAD_SUBTYPE    = 4, ///< Vendor subtype not supported
    BR_EPERM          = 5, ///< Permisssions error
    BR_BAD_LEN        = 6, ///< Wrong message length
    BR_BUFFER_EMPTY   = 7, ///< Specified buffer has already been used
    BR_BUFFER_UNKNOWN = 8  ///< Specified buffer does not exist
  };

  enum Bad_action {
    BA_BAD_TYPE        = 0, ///< Unknown action type
    BA_BAD_LEN         = 1, ///< Wrong length for action
    BA_BAD_VENDOR      = 2, ///< Unknown vendor id
    BA_BAD_VENDOR_TYPE = 3, ///< Unknown action type for vendor id
    BA_BAD_OUT_PORT    = 4, ///< Problem validating output action
    BA_BAD_ARGUMENT    = 5, ///< Bad action argument
    BA_EPERM           = 6, ///< Permissions error
    BA_TOO_MANY        = 7, ///< Too many actions
    BA_BAD_QUEUE       = 8  ///< Problem validating the input queue
  };

  enum Flow_mod_failed {
    FMF_ALL_TABLES_FULL   = 0, ///< All tables are full
    FMF_OVERLAP           = 1, ///< Cannot add overlapped flow
    FMF_EPERM             = 2, ///< Perminssions error
    FMF_BAD_EMERG_TIMEOUT = 3, ///< Operation timed out
    FMF_BAD_COMMAND       = 4, ///< Unknown command
    FMF_UNSUPPORTED       = 5  ///< Unsupported action list
  };

  enum Port_mod_failed {
    PMF_BAD_PORT    = 0, ///< Invalid port
    PMF_BAD_HW_ADDR = 1  ///< Hardware address is wrong
  };

  enum Queue_op_failed {
    QOF_BAD_PORT  = 0, ///< Invalid port
    QOF_BAD_QUEUE = 1, ///< Invalid queue
    QOF_EPERM     = 2  ///< Permissions error
  };
  
  Uint16 type;
  Uint16 code;
  Buffer data;
};

struct Echo_request {
  static constexpr Message_type Kind = ECHO_REQUEST;
  
  Buffer data;
};

struct Echo_reply {
  static constexpr Message_type Kind = ECHO_REPLY;
  
  Buffer data;
};

struct Vendor {
  static constexpr Message_type Kind = VENDOR;

  Uint32 vendor_id;
  Buffer data;
};

/// The features class provides scoping for capabilities and supported actions.
struct Features {
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
};

struct Feature_request { 
  static constexpr Message_type Kind = FEATURE_REQUEST;
};

struct Feature_reply {
  static constexpr Message_type Kind = FEATURE_REPLY;

  using Capability = Features::Capability;
  using Action = Features::Action;
  
  Uint64     datapath_id;
  Uint32     nbuffers;
  Uint8      ntables;
  Capability capabilities;
  Action     actions;
  Port_list  ports;
};

struct Config {
  enum Flags : Uint16 {
    FRAG_NORMAL = 0x0000, 
    FRAG_DROP   = 0x0001, 
    FRAG_REASM  = 0x0002, 
    FRAG_MASK   = 0x0003
  };
};

struct Get_config_request {
  static constexpr Message_type Kind = GET_CONFIG_REQUEST;
};

struct Get_config_reply {
  static constexpr Message_type Kind = GET_CONFIG_REPLY;

  using Flags = Config::Flags;
  
  Flags  flags;
  Uint16 miss_send_len;
};

struct Set_config {
  static constexpr Message_type Kind = SET_CONFIG;

  using Flags = Config::Flags;
  
  Flags  flags;
  Uint16 miss_send_len;
};


/// A Packet_in message is sent from the switch to the controller to
/// indicate a miss in the flow tables or because a specific action
/// has been requested.
struct Packet_in {
  static constexpr Message_type Kind = PACKET_IN;

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
  static constexpr Message_type Kind = FLOW_REMOVED;
  
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

/// The Port_status message is sent from the switch to the controller to
/// indicate a change to the status of a port.
struct Port_status {
  static constexpr Message_type Kind = PORT_STATUS;

  enum Reason : Uint8 {
    ADD = 0x00, 
    DELETE = 0x01, 
    MODIFY = 0x02
  };

  Reason reason;
  Port    port;
};

/// A Packet_out message is sent from the controller to the switch in order
/// to inject a packet into a flow or release a packet from a buffer.
struct Packet_out {
  static constexpr Message_type Kind = PACKET_OUT;

  Uint32      buffer_id;
  Port::Id    port;
  Uint16      actions_len;
  Action_list actions;
  Buffer      data;
};

/// A Flow_mod message is sent from the controller to the switch to modify
/// an entry in a flow table.
struct Flow_mod {
  static constexpr Message_type Kind = FLOW_MOD;

  enum Command : Uint16 {
    ADD           = 0x0000, 
    MODIFY        = 0x0001, 
    MODIFY_STRICT = 0x0002, 
    DELETE        = 0x0003,
    DELETE_STRICT = 0x0004
  };

  enum Flags : Uint16 {
    SEND_FLOW_REM = 0x0001, 
    CHECK_OVERLAP = 0x0002, 
    EMERG         = 0x0003
  };

  Match       match;
  Uint64      cookie;
  Command     command;
  Uint8       idle_timeout;
  Uint8       hard_timeout;
  Uint8       priority;
  Uint32      buffer_id;
  Port::Id    out_port;
  Flags       flags;
  Action_list actions;
};

/// A Port_mod message is sent from the controller to the switch to modify
/// the state of a port.
struct Port_mod {
  static constexpr Message_type Kind = PORT_MOD;

  Port::Id       port;
  Mac_addr       hw_addr;
  Port::Config   config;
  Port::Config   mask;
  Port::Features advertised;
};

/// A stats-request message is sent from the controller to the switch to
/// request information and statistics about some aspect of a switch.
struct Stats_request {
  static constexpr Message_type Kind = STATS_REQUEST;

  using Header = Stats_header;
  using Payload = Stats_request_payload;

  Header header;
  Payload payload;
};

/// A stats-response message is returned from the switch to the controller
/// containing information and statistics.
struct Stats_reply {
  static constexpr Message_type Kind = STATS_REPLY;

  using Header = Stats_header;
  using Payload = Stats_reply_payload;

  Stats_reply();
  ~Stats_reply();

  Header header;
  Payload payload;
};

struct Barrier_request { 
  static constexpr Message_type Kind = BARRIER_REQUEST;
};

struct Barrier_reply {
  static constexpr Message_type Kind = BARRIER_REPLY;
};

/// A queue-config request is sent from the controller to the switch to 
/// get configuration information about a queue.
struct Queue_config_request {
  static constexpr Message_type Kind = QUEUE_GET_CONFIG_REQUEST;

  Port::Id port;
};

/// A queue-config reploy is returned from the switch to the controller to
/// communicate queue configuration.
struct Queue_config_reply {
  static constexpr Message_type Kind = QUEUE_GET_CONFIG_REPLY;

  Port::Id port;
  Queue_list queues;
};

/// The message class embodies a specific kind of OpenFlow message.
union Payload {
  // This union contains non-trivial members
  Payload() { }
  ~Payload() { }
  
  Hello                hello;
  Error_message        error;
  Echo_request         echo_req;
  Echo_reply           echo_rep;
  Vendor               vendor;
  Feature_request      feature_req;
  Feature_reply        feature_rep;
  Get_config_request   get_config_req;
  Get_config_reply     get_config_rep;
  Set_config           set_config;
  Packet_in            packet_in;
  Flow_removed         flow_removed;
  Port_status          port_status;
  Packet_out           packet_out;
  Flow_mod             flow_mod;
  Port_mod             port_mod;
  Stats_request        stats_request;
  Stats_reply          stats_reply;
  Barrier_request      barrier_req;
  Barrier_reply        barrier_rep;
  Queue_config_request queue_config_request;
  Queue_config_reply   queue_config_reply;
};

struct Header {
  Header() = default;
  Header(Message_type, Uint16, Uint32);

  Version_type version;
  Message_type type;
  Uint16 length;
  Uint32 xid;
};

/// A message comprised of an OpenFlow header and its payload.
struct Message {
  Header header;
  Payload payload;
};

// Operations
void construct(Payload&, Message_type);
void destroy(Payload&, Message_type);

// Protocol

using ofp::to_view;
using ofp::from_view;
using ofp::bytes;

std::size_t bytes(const Hello&);
std::size_t bytes(const Error_message&);
std::size_t bytes(const Echo_request&);
std::size_t bytes(const Echo_reply&);
std::size_t bytes(const Vendor&);
std::size_t bytes(const Feature_request&);
std::size_t bytes(const Feature_reply&);
constexpr std::size_t bytes(const Get_config_request&);
constexpr std::size_t bytes(const Get_config_reply&);
constexpr std::size_t bytes(const Set_config&);
std::size_t bytes(const Packet_in&);
constexpr std::size_t bytes(const Flow_removed&);
constexpr std::size_t bytes(const Port_status&);
std::size_t bytes(const Packet_out&);
std::size_t bytes(const Flow_mod&);
constexpr std::size_t bytes(const Port_mod&);
std::size_t bytes(const Stats_request&);
std::size_t bytes(const Stats_reply&);
constexpr std::size_t bytes(const Barrier_request&);
constexpr std::size_t bytes(const Barrier_reply&);
std::size_t bytes(const Queue_config_request&);
std::size_t bytes(const Queue_config_reply&);
std::size_t bytes(const Payload&, Message_type);
constexpr std::size_t bytes(const Header&);
std::size_t bytes(const Message&);

Error to_view(View&, const Hello&);
Error to_view(View&, const Error_message&);
Error to_view(View&, const Echo_request&);
Error to_view(View&, const Echo_reply&);
Error to_view(View&, const Vendor&);
Error to_view(View&, const Feature_request&);
Error to_view(View&, const Feature_reply&);
Error to_view(View&, const Config&);
Error to_view(View&, const Packet_in&);
Error to_view(View&, const Flow_removed&);
Error to_view(View&, const Port_status&);
Error to_view(View&, const Packet_out&);
Error to_view(View&, const Flow_mod&);
Error to_view(View&, const Port_mod&);
Error to_view(View&, const Stats_request&);
Error to_view(View&, const Stats_reply&);
Error to_view(View&, const Barrier_request&);
Error to_view(View&, const Barrier_reply&);
Error to_view(View&, const Queue_config_request&);
Error to_view(View&, const Queue_config_reply&);
Error to_view(View&, const Payload&, Message_type);
Error to_view(View&, const Header&);
Error to_view(View&, const Message&);

Error from_view(View&, Hello&);
Error from_view(View&, Error_message&);
Error from_view(View&, Echo_request&);
Error from_view(View&, Echo_reply&);
Error from_view(View&, Vendor&);
Error from_view(View&, Feature_request&);
Error from_view(View&, Feature_reply&);
Error from_view(View&, Config&);
Error from_view(View&, Packet_in&);
Error from_view(View&, Flow_removed&);
Error from_view(View&, Port_status&);
Error from_view(View&, Packet_out&);
Error from_view(View&, Flow_mod&);
Error from_view(View&, Port_mod&);
Error from_view(View&, Stats_request&);
Error from_view(View&, Stats_reply&);
Error from_view(View&, Barrier_request&);
Error from_view(View&, Barrier_reply&);
Error from_view(View&, Queue_config_request&);
Error from_view(View&, Queue_config_reply&);
Error from_view(View&, Payload&, Message_type);
Error from_view(View&, Header&);
Error from_view(View&, Message&);

// Validation

constexpr bool is_valid(Version_type);
constexpr bool is_valid(Message_type);

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#include "message.ipp"

#endif
