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

#include "machine.hpp"

namespace freeflow {
namespace ofp {
namespace v1_0 {

/// Called when the state machine enters its intial state.
/// This sends the hello message.
Error
Machine::on_initial() {
  state_ = INITIAL;
  return send_hello();
}

/// Called when the state machine enters version negotiation. Here,
/// v is the version requested by the remote switch. If the version 
/// is not supported, return an error, and associate  the requested version.
Error
Machine::on_negotiate(int v) {
  // Enter version nen
  state_ = NEGOTIATE;
  if (v != VERSION)
    return {make_error_code(errc::incompatible_version), v};
  else
    return {};
}

Error
Machine::on_discover() {
  state_ = DISCOVER;
  return send_feature_request();
}

/// Called when the state machine enters its fianl state.
/// This releases all resources prior to shutdown.
Error
Machine::on_final() {
  return {};
}

Error
Machine::on_message() {
  /// Peek at the message header.
  Header h;
  if(Trap err = ch_.recv.peek_msg(h))
    return err;
  
  // The handling depends on the state.
  switch (state_) {
  case INITIAL: return on_initial_message(h);
  default: return {};
  }
  return {};
}

// Tansition out of the initial state based on the message received.
Error
Machine::on_initial_message(const Header& h) {
  if (h.type == HELLO)
    return on_initial_hello(h);
  else
    return on_initial_other();
}

// Enter the 
Error
Machine::on_initial_hello(const Header& h) {
  Hello m;
  if (Trap err = ch_.recv.pop_msg(h, m))
    return err;
  on_negotiate(h.version);
  return {};
}


Error
Machine::on_initial_other() {
  return {};
}

} // namespace v1_0
} // namespace ofp
} // namespace nocontrol
