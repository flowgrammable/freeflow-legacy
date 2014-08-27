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

#ifndef FREEFLOW_PORT_HPP
#define FREEFLOW_PORT_HPP

#include <string>

#include <freeflow/sdn/queue.hpp>
#include <freeflow/sys/data.hpp>

namespace freeflow {

/// A MAC address is a 48-bit identifier that uniquely identifies the device.
struct Mac_addr {
  Uint8 addr[6];
};

///The Port structure represents a port on a switch ...
///
/// todo\ Doesn't this need some of the info from the ofp Config or ofp State?
struct Port {
  static constexpr Uint32 MAX         = 0xffffff00;
  static constexpr Uint32 IN_PORT     = 0xfffffff8;
  static constexpr Uint32 TABLE       = 0xfffffff9;
  static constexpr Uint32 NORMAL      = 0xfffffffa;
  static constexpr Uint32 FLOOD       = 0xfffffffb;
  static constexpr Uint32 ALL         = 0xfffffffc;
  static constexpr Uint32 CONTROLLER  = 0xfffffffd;
  static constexpr Uint32 LOCAL       = 0xfffffffe;
  static constexpr Uint32 NONE        = 0xffffffff; 
  // static constexpr Uint32 ANY         = 0xffffffff; 
  // TODO: is there any semantic difference between ANY and NONE? For now
  // they will be treated the same.

  /// The Mode enumeration represents the communication system of 
  /// the port. It can either be half-duplex or full-duplex.
  enum Mode { 
    HALF_DUPLEX, // Two-way communication, one direction at a time
    FULL_DUPLEX  // Two-way communication, both directions simultaneously
  };

  /// The Medium enum represents the cable medium for the port
  enum Medium { 
    COPPER,
    FIBER
  };

  /// The Port::Features structure represents the features of the port.
  /// This includes the speed, communication mode and link information 
  /// such as the type of medium, auto-negotiation state, and pause.
  ///
  /// An instance of this structure exists for the current port features, 
  /// the features advertised by the port, the features supported by the
  /// port, and the features of the port advertised by peers.
  struct Features {
    int    speed;     
    Mode   mode;      
    Medium medium;    
    bool   auto_neg;  
    bool   pause;     
    bool   pause_asym;
  };

  Uint32      port_number; // Physical port number
  Queues      queues;      // Queues linked to this port
  Mac_addr    hw_addr;     // From FeatureRes.ports[]
  std::string name;        // From FeatureRes.ports[]
  Features    current;     // From FeatureRes.ports[].supported
  Features    advertised;  // From FeatureRes.ports[].supported
  Features    supported;   // From FeatureRes.ports[].supported
  Features    peer;        // From FeatureRes.ports[].supported
};

/// The Ports structure represents a sequence of ports and some information 
/// related to them.
struct Ports : public std::map<Uint32, Port> {
  bool port_stats;  // From FeatureRes.capabilities
  bool stp;         // From FeatureRes.capabilities
  bool all;         // True
  bool controller;  // True
  bool local;       // True
  bool table;       // True
  bool in_port;     // True
  bool normal;      // ?
  bool flood;       // ?
  bool none;        // True
};

/// The Port_stats_entry structure contains all of the counters for a
/// port. Any counters that are not supported have all bits set to 1.
///
/// todo\ overload bool operators for these counters to check whether
/// they are supported or not
struct Port_stats_entry {
  Uint32 port_number;   // Physical port number
  Uint64 rx_packets;    // Number of received packets
  Uint64 tx_packets;    // Number of transmitted packets
  Uint64 rx_bytes;      // Number of received bytes
  Uint64 tx_bytes;      // Number of transmitted bytes
  Uint64 rx_dropped;    // Number of packets dropped by RX
  Uint64 tx_dropped;    // Number of packets dropped by TX
  Uint64 rx_errors;     // Total number of receive errors
  Uint64 tx_errors;     // Total number of transmit errors
  Uint64 rx_frame_err;  // Number of frame alignment errors
  Uint64 rx_over_err;   // Number of packets with RX overrun
  Uint64 rx_crc_err;    // Number of CRC errors
  Uint64 collisions;    // Number of collisions
  Uint32 duration_sec;  // Seconds port has been alive
  Uint32 duration_nsec; // Nanoseconds alive beyond duration_sec
};

/// Port_stats represents a table of Port_stats_entry objects in the 
/// form of a port_number to Port_stats_entry mapping. 
using Port_stats = std::map<Uint32, Port_stats_entry>;

} // namespace freeflow

#include <freeflow/sdn/port.ipp>

#endif