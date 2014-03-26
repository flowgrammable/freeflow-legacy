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
Connection::open() {
  proto_ = new ofp::Protocol();
  
  // FIXME: Error checking.
  proto_->open();
  return write();
}

// Shutdown the state machine and delete the handler.
bool 
Connection::close() { 
  delete this;
  return true; 
}


Result 
Connection::on_read() {
  // Try to read the next message.
  Result r1 = read();
  if (r1 != CONTINUE)
    return r1;
  
  // Notify the protocol of a change.
  proto_->message();
  
  // Try to write the resulting messages.
  Result r2 = write();
  if (r2 != CONTINUE)
    return r2;
  
  return CONTINUE;
}

Result
Connection::on_write() { return CONTINUE; }

// FIXME: This is incredibly brittle, and its slow. First, each message
// requires 2 reads and 2 allocations (ouch). Second, we may not actually
// receive everything in each read, meaning we're going to miss data.
// Third, error handling is totally broken.
//
// We can alleviate the read errors by re-incorporating the fd_utils
// header with the resource header.
//
// If we had a recombinant buffers, we could eliminate the need to
// allocate or issue multiple reads.
Result
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
    return STOP;

  // Read the remainder of the message. 
  // FIXME: I may not read hdr.length - 8 bytes.
  buf.resize(hdr.length);
  rc().read(buf.data() + 8, hdr.length - 8);

  // Queue the buffer.
  proto_->read.put_buffer(std::move(buf));
  return CONTINUE;
}

Result
Connection::write() {
  while (not proto_->write.empty()) {
    Buffer b;
    proto_->write.get_buffer(b);
    rc().write(b);
  }
  return CONTINUE;
}

} // namespace nocontrol

