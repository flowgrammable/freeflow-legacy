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
#include <freeflow/proto/ofp/v1.0/protocol.hpp>

#include "protocol.hpp"

namespace freeflow {
namespace ofp {

bool 
Protocol::on_open(Reactor& r) { 
  state_ = HANDSHAKE;

  // Send the hello message.
  put_message(v1_0::Hello());

  // Install a timeout timer.
  r.schedule_timer(handler_, timer_, config_.hello_timeout);

  return true;
}

bool
Protocol::on_close(Reactor&) {
  return true;
}

bool
Protocol::on_recv(Reactor& r) {
  if (state_ == HANDSHAKE)
    return initial_recv(r);
  return true;
}

bool
Protocol::on_time(Reactor& r, int t) { 
  if (state_ == HANDSHAKE)
    return initial_time(r);
  return true;
}

bool
Protocol::initial_recv(Reactor& r) {
  Header h;
  read.peek_header(h);

  // FIXME: Error handling. Also, make sure that HELLO is the same value
  // for each protocol. It could possibly change from version to version.
  if (h.type != v1_0::HELLO)
    return false;

  // proto_ = negotiate(h.version);
  // to_feature();
  return true;
}

bool
Protocol::initial_time(Reactor& r) {
  // FIXME: We need to shutdown.
  return true;
}


// // open -> hello
// void
// Protocol::hello() {
// }

// void
// Protocol::to_feature() { 
//   proto_->to_feature(); 
//   state_ = WAIT_FEATURE;
// }

// void
// Protocol::wait_feature() {
//   proto_->wait_feature(); 
// }

// void
// Protocol::to_established() {
//   // FIXME: Set up requests for timers.
// }


// Configure the protocol version.
// Note that version negotiation should not fail.
//
// FIXME: We have to do some stuff with version bitmaps
// here... not sure what.
Protocol::Version*
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