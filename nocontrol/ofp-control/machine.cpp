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
/// Transition immediately to the negotiating state and send
/// the hello message.
Error
Machine::on_initial() {
  state_ = NEGOTIATE;
  return send_hello();
}

/// Called when the state machine enters version negotiation. Here,
/// v is the version requested by the remote switch. If the version 
/// is not supported, return a system error.
Error
Machine::on_negotiate(int v) {
  if (v != VERSION)
    return {make_error_code(std::errc::protocol_not_supported), v};
  else
    return {};
}

/// Called when negotiation has failed, this sends an error messgae
/// rejecting the incompatible version.
Error
Machine::on_reject() {
  return send_error(Error_message::HF_INCOMPATIBLE);
}

/// Enter into the feature discovery state. Send a feature request.
Error
Machine::on_discover() {
  state_ = DISCOVER;
  return request_features();
}

/// Enter into the processing state. There are no protocol actions 
/// associtaed with this transition.
Error
Machine::on_process() {
  state_ = PROCESS;
  return {};
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
  case NEGOTIATE: return on_negotiate_message(h);
  case DISCOVER: return on_discover_message(h);
  case PROCESS: return on_process_message(h);
  default: return {};
  }
  return {};
}

// -------------------------------------------------------------------------- //
// Version negotiation transitions

/// Receiving a hello message causes a transition to the negotiating
/// state.
///
/// Receiving any messagte other than hello results in disconnection.
/// Note that this could be an Error message indicating version negotiation
/// failure.
Error
Machine::on_negotiate_message(const Header& h) {
  if (h.type != HELLO)
    return make_error_code(std::errc::protocol_error);

  Hello m;
  if (Trap err = ch_.recv.pop_msg(h, m))
    return err;
  return on_negotiate(h.version);
}

/// Receiving a timeout now results in protocol termination.
Error
Machine::on_negotiate_timeout() {
  return make_error_code(std::errc::timed_out);
}


// -------------------------------------------------------------------------- //
// Feature discovery transitions

/// Receiving a feature reply causes a transition to the processing
/// state. Here, we create a switch object corresponding to the
/// the features contained in the reply.
///
/// Receiving any messagte other than a feature reply results in 
/// disconnection. This could be an Error message indicating version 
/// negotiation failure or feature negotiation failure.
Error
Machine::on_discover_message(const Header& h) {
  if (h.type != FEATURE_REPLY)
    return make_error_code(std::errc::protocol_error);

  Feature_reply m;
  if (Trap err = ch_.recv.pop_msg(h, m))
    return err;

  // FIXME: Actually build or cnfigure the switch object.

  return on_process();
}

/// Receiving a timeout now results in protocol termination.
Error
Machine::on_discover_timeout() {
  return make_error_code(std::errc::timed_out);
}


// -------------------------------------------------------------------------- //
// Processing state transitions

// FIXME: Do something here.
Error
Machine::on_process_message(const Header& h) {
  return {};
}


} // namespace v1_0
} // namespace ofp
} // namespace nocontrol
