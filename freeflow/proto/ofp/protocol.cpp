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

#include <algorithm>

#include <freeflow/proto/ofp/v1.0/message.hpp>

#include "protocol.hpp"

namespace freeflow {
namespace ofp {

void 
Protocol::open() { to_hello(); }

void 
Protocol::close() { }

void
Protocol::message() {
  if (state_ == WAIT_HELLO)
    wait_hello();
  else if (state_ == WAIT_FEATURE)
    wait_feature();
  else
    // Whatever else we do depends on the protocol version.
    return;
}

void
Protocol::time() { }

void
Protocol::to_hello() {
  // FIXME: The hello message needs to be the highest version
  // of the protocol supported.
  v1_0::Hello m;
  v1_0::Header h(v1_0::HELLO, bytes(m), 0);
  write.put_message(h, m);
  state_ = WAIT_HELLO;
}

// Respond to a message while waiting for a hello message.
void
Protocol::wait_hello() {
  Header h;
  read.peek_header(h);

  // FIXME: Error handling. Also, make sure that HELLO is the same value
  // for each protocol. It could possibly change from version to version.
  if (h.type != v1_0::HELLO)
    return;

  proto_ = negotiate(h.version);
  to_feature();
}

void
Protocol::to_feature() { 
  proto_->to_feature(); 
  state_ = WAIT_FEATURE;
}

void
Protocol::wait_feature() { 
  proto_->wait_feature(); 
}


// Configure the protocol version.
// Note that version negotiation should not fail.
//
// FIXME: We have to do some stuff with version bitmaps
// here... not sure what.
Protocol::Handler*
Protocol::negotiate(Uint8 v) {
  version_ = std::min(config_.version, v);
  switch (version_) {
  case 0:
    return new v1_0::Handler(*this);
  default:
    return nullptr; // unreachable
  }
}


} // namespace ofp
} // namespace freeflow