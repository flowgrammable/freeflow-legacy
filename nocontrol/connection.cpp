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

#include "connection.hpp"

using namespace freeflow;

namespace nocontrol {

// Create a version negotiation state machine so we know what version
// we should accept.
bool
Connection::on_open(Reactor&) {
  proto_ = new ofp::Protocol();
  
  // FIXME: Error checking.
  proto_->open();
  return write();
}

// Shutdown the state machine and delete the handler.
bool 
Connection::on_close(Reactor&) { 
  delete this;
  return true; 
}

/// When data is available, read as many messages as possibly and send
/// them to the state machine.
bool
Connection::on_read(Reactor&) {
  if (not read())
    return false;
  proto_->message();
  if (not write())
    return false;
  return true;
}


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
Connection::read() {
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
  proto_->read.put_buffer(std::move(buf));
  return true;
}

bool
Connection::write() {
  while (not proto_->write.empty()) {
    Buffer b;
    proto_->write.get_buffer(b);
    rc().write(b);
  }
  return true;
}

} // namespace nocontrol

