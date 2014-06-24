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

#include <freeflow/proto/ofp/ofp.hpp>
#include <freeflow/sdn/queue.hpp>
#include <freeflow/sys/data.hpp>


namespace freeflow {


///The Port structure represents a port on a switch ...
struct Port {

  // static constexpr Uint32 MAX         = 0xffffff00;
  // static constexpr Uint32 IN_PORT     = 0xfffffff8;
  // static constexpr Uint32 TABLE       = 0xfffffff9;
  // static constexpr Uint32 NORMAL      = 0xfffffffa;
  // static constexpr Uint32 FLOOD       = 0xfffffffb;
  // static constexpr Uint32 ALL         = 0xfffffffc;
  // static constexpr Uint32 CONTROLLER  = 0xfffffffd;
  // static constexpr Uint32 LOCAL       = 0xfffffffe;
  // static constexpr Uint32 NONE        = 0xffffffff; // how to handle
  // static constexpr Uint32 ANY         = 0xffffffff; // these two??

  /// The Mode enumeration represents the communication system of 
  /// the port. It can either be half-duplex or full-duplex.
  enum Mode { 
    HALF_DUPLEX,  // Two-way communication with one direction at a time
    FULL_DUPLEX   // Two-way communication with both directions simultaneously
  };

  /// The Medium enum represents the cable medium for the port
  enum Medium { 
    COPPER,
    FIBER
  };

  Uint32        port_number;
  Queues        queues;

  ofp::Mac_addr hw_addr;    // From FeatureRes.ports[]
  std::string   name;       // From FeatureRes.ports[]
  int           speed;      // From FeatureRes.ports[].supported
  Mode          mode;       // From FeatureRes.ports[].supported
  Medium        medium;     // From FeatureRes.ports[].supported
  bool          auto_neg;   // From FeatureRes.ports[].supported
  bool          pause;      // From FeatureRes.ports[].supported
  bool          pause_asym; // From FeatureRes.ports[].supported

};

/// The Ports structure represents a list of ports and some information 
/// related to them.
struct Ports {
  bool port_stats;  // From FeatureRes.capabilities
  bool stp;         // From FeatureRes.capabilities
  bool all;         // 
  bool controller;  // 
  bool local;       // 
  bool table;       // 
  bool in_port;     // 
  bool normal;      // 
  bool flood;       // 
  bool none;        // 

  std::vector<Port> ports;

};


// The Port_stats structure contains all of the counters for a port.
// Any counters that are not supported have all bits set to 1.
struct Port_stats {
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

} // namespace freeflow

#include <freeflow/sdn/port.ipp>

#endif