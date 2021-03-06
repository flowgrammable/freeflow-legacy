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

#include <iostream>

#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/ofp.hpp>

#include "openflow.hpp"
#include "machine.hpp"

using namespace freeflow;

namespace nocontrol {

/// Create a state machine and initiate version negotiation. 

/// Note that the state machine does not "drive" its own state during
/// the startup states of communication. Instead, the event handler
/// drives because the state machine may be replaced when the first
/// hello mesage is recieved.
bool
Ofp_handler::on_open() {
  Write_on_exit g(*this);
  std::cout << "* Open OFP connection\n";

  // Create the state machine. Note that any errors sending hello
  // cannot be communicated as errors to the connected switch.
  //
  // FIXME: Create the state machine for the greatest version of the
  // protocol supported.
  sm_ = new ofp::v1_0::Machine(ch_, &ctrl_);
  if (not sm_->enter_hello_wait())
    return false;
  else
    return true;
}

/// Destroy the state machine. Note that messages sent in a state
/// machine's destructor are not sent.
bool 
Ofp_handler::on_close() {
  std::cout << "* Close OFP connection\n";

  // FIXME: Send a message to the SM?

  delete sm_;
  return true; 
}

/// When data is available, read as many messages as possibly and send
/// them to the state machine.
///
/// \todo Error checking.
bool
Ofp_handler::on_read() {
  Write_on_exit g(*this);
  if (not read()) 
    return false;

  // Process the message.
  Error err = sm_->on_message();

  // If version negotation returns an error, then we need to replace
  // the state machine with a new version.
  if (sm_->in_negotiation())
    return check_version(err);

  return (bool)err;
}

/// When a timeout occurs, notify the protocol of the expired timer.
bool
Ofp_handler::on_time(int t) {
  // Write_on_exit g(*this);
  // return proto_->on_time(reactor(), t);
  return true;
}

/// Check the result of receiving a mesage that triggered version
/// negotiation. If the error indicates that the protocol is not
/// supported, try changing the protocol version. If any other error
/// occurs, drop the connection.
///
/// After the approproate protocol version is set, enter its
/// discovery phase.
bool
Ofp_handler::check_version(ff::Error err) {
  if (not err) {
    // On any other error, drop the connection.
    if (err.code() != std::errc::protocol_not_supported)
      return false;

    // Try changing versions. If that fails, drop the connection.
    if (not change_version(err.data())) {
      sm_->enter_version_rejection();
      return false;
    }
  }

  // Enter feature discovery.
  if (Trap err = sm_->enter_feature_wait())
    return false;
  else
    return true;
}

/// Replace the state machine with one of the specified version.
/// This need to swap out the internal state maintained by the
/// existing state machine.
///
/// \todo Actually implement this function.
bool
Ofp_handler::change_version(int v) {
  std::cout << "* Request OFP version " << v << '\n';
  return false;
}

// Read from the socket to put message data into the read queue.
//
// FIXME: This is incredibly brittle, and its slow. First, each message
// requires two reads and two allocations. Second, we may not actually
// receive everything in each read, meaning we're going to miss data.
// Third, error handling is totally broken.
//
// We can alleviate the read errors by re-incorporating the fd_utils
// header with the resource header.
//
// If we had a recombinant buffers, we could eliminate the need to
// allocate or issue multiple reads.
bool
Ofp_handler::read() {
  // Sample enough to read only the header.
  // FIXME: I may not read 8 bytes.
  Buffer buf(8);
  rc().read(buf);

  // Cheat by reading only the header. If there's a here, we may 
  // need to stop. We may also need to send an error message.
  View v(buf);
  ofp::Header hdr;
  if (Trap err = from_view(v, hdr))
    return false;

  // Read the remainder of the message. 
  // FIXME: I may not read hdr.length - 8 bytes.
  buf.resize(hdr.length);
  rc().read(buf.data() + 8, hdr.length - 8);

  // Queue the buffer.
  ch_.recv.push_buf(std::move(buf));
  return true;
}

// Write all messages to the socket.
bool
Ofp_handler::write() {
  while (not ch_.send.empty()) {
    Buffer b;
    ch_.send.pop_buf(b);

    // FIXME: Error handling
    rc().write(b);
  }
  return true;
}



// -------------------------------------------------------------------------- //
// Switch configuration

/// Configure the features and capabilities of a switch.
inline void 
datapath_config(Datapath& dp, const ofp::v1_0::Feature_reply& r) {
  // Configure datapath members
  dp.datapath_id   = r.datapath_id;
  dp.ip_reassembly = get_bit(r.capabilities, 5);

  // Configure Ports
  // TODO: many fields still need to be set... this is NOT done yet.. at all
  dp.ports.port_stats = get_bit(r.capabilities, 2);
  dp.ports.stp = get_bit(r.capabilities, 3);
  // TODO: set up Port_stats table (map). Should a Stats_req be sent to do this?
  // if (dp.ports.port_stats) // set up Port_stats
  for (const ofp::v1_0::Port& port : r.ports) {
    Port p;
    p.port_number = port.port_id;
    set_mac_addr(p.hw_addr, port.hw_addr);
    p.name = port.name.str();
    features_config(p.current, port.current);
    features_config(p.advertised, port.advertised);
    features_config(p.supported, port.supported);
    features_config(p.peer, port.peer);
    // TODO: set up Queues before emplacing to the map
    dp.ports.emplace(p.port_number, p);
  }

  // Configure Flow_tables
  // TODO: actually configure the tables...
  dp.flow_tables.flow_stats = get_bit(r.capabilities, 0);
  dp.flow_tables.table_stats = get_bit(r.capabilities, 1);
}

/// Set the features of a port based on the features contained
/// in an OpenFlow v1.0 Feature_reply message.
inline void
features_config(Port::Features& to, const ofp::v1_0::Port::Features& from) {

  // 10 MB half duplex
  if (get_bit(from, 0)) {
    to.speed = 10000;
    to.mode  = Port::HALF_DUPLEX;
  }

  // 10 MB full duplex
  else if (get_bit(from, 1)) {
    to.speed = 10000;
    to.mode  = Port::FULL_DUPLEX;
  }

  // 100 MB half dulex
  else if (get_bit(from, 2)) {
    to.speed = 100000; 
    to.mode  = Port::HALF_DUPLEX;
  }

  // 100 MB full dulex
  else if (get_bit(from, 3)) {
    to.speed = 100000;
    to.mode  = Port::FULL_DUPLEX;
  }
  
  // 1 GB half dulex
  else if (get_bit(from, 4)) {
    to.speed = 1000000;
    to.mode  = Port::HALF_DUPLEX;
  }
  
  // 1 GB full dulex
  else if (get_bit(from, 5)) {
    to.speed = 1000000;
    to.mode  = Port::FULL_DUPLEX;
  }

  // 10 GB full duplex
  else if (get_bit(from, 6)) {
    to.speed = 10000000;
    to.mode  = Port::FULL_DUPLEX;
  }

  // else error? v1.3 uses max_speed or curr_speed but v1.0 doesn't have them

  // Copper medium
  if (get_bit(from, 7)) 
    to.medium = Port::COPPER;

  // Fiber medium
  else if (get_bit(from, 8))
    to.medium = Port::FIBER;

  // else error? is this data necessary enough to warrant an error?

  to.auto_neg   = get_bit(from, 9);
  to.pause      = get_bit(from, 10);
  to.pause_asym = get_bit(from, 11);
}

} // namespace nocontrol

