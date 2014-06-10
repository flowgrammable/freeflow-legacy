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

#include <freeflow/sdn/queue.hpp>
#include <freeflow/sys/data.hpp>

namespace freeflow {



struct Port {
  Uint32 port_number;
  Queues queues;
};

struct Ports {
  std::vector<Port> ports;

};


// todo: does it matter that port stats entry does not have padding 
// after id in freeflow/proto/ofp/v1.0/stats.hpp ?

// Structure of an ofp port stats response. Unsupported counters have
// all bits set to 1.
// 
// Is this needed?
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