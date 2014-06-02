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

/// Create a new switch in the SDN model, and an OpenFlow state machnine
/// to manage this connection.
///
/// \todo Error checking.
inline bool
Ofp_handler::on_open() {
  Write_on_exit g(*this);
  std::cout << "* Open OFP connection\n";

  // Create the state machine.
  // FIXME: Create the state machine for the greatest version of the
  // protocol supported.
  sm_ = new Machine(ch_, &ctrl_);

  // if (Trap err = sm_->)

  return true;
}

/// Shutdown the state machine and delete the handler.
inline bool 
Ofp_handler::on_close() {
  Write_on_exit g(*this);
  std::cout << "* close OFP connection\n";

  // Destroy the existing switch object.


  return true; 
}

/// When data is available, read as many messages as possibly and send
/// them to the state machine.
///
/// \todo Error checking.
inline bool
Ofp_handler::on_read() {
  // Write_on_exit g(*this);
  // if (not read()) 
  //   return false;
  // return proto_->on_recv(reactor());

  char buf[2048];
  ff::System_result res = rc().read(buf, 2048);
  if (res and res.value() == 0)
    return false;
  return true;
}

/// When a timeout occurs, notify the protocol of the expired timer.
inline bool
Ofp_handler::on_time(int t) {
  // Write_on_exit g(*this);
  // return proto_->on_time(reactor(), t);
  return true;
}


// Read from the socket to put message data into the read queue.
//
// FIXME: This is incredibly brittle, and its slow. First, each message
// requires twp reads and two allocations. Second, we may not actually
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

} // namespace nocontrol

