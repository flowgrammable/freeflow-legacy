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

/// Called when the state machine begins waiting for a hello message.
/// Send a hello message to the switch.
Error
Machine::enter_hello_wait() {
  state_ = HELLO_WAIT;
  return send_hello();
}

/// Called when the state machine enters version negotiation. Here,
/// v is the version requested by the remote switch. If the version 
/// is not supported, return a system error.
Error
Machine::enter_version_negotiation() { 
  state_ = NEGOTIATE; 
  return {};
}

/// Called when negotiation has failed, this sends an error messgae
/// rejecting the incompatible version.
Error
Machine::enter_version_rejection() {
  state_ = REJECT;
  return send_error(Error_message::HF_INCOMPATIBLE);
}

/// Enter into the feature discovery state. Send a feature request.
Error
Machine::enter_feature_wait() {
  state_ = FEATURE_WAIT;
  return request_features();
}

/// Enter into the processing state. There are no protocol actions 
/// associtaed with this transition.
Error
Machine::enter_established() {
  state_ = ESTABLISHED;
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
  case HELLO_WAIT: return on_hello_wait_message(h);
  case FEATURE_WAIT: return on_feature_wait_message(h);
  case ESTABLISHED: return on_established_message(h);
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
Machine::on_hello_wait_message(const Header& h) {
  if (h.type != HELLO)
    return make_error_code(std::errc::protocol_error);

  // Enter the version negotiation state.
  enter_version_negotiation();

  // Decode the error.
  Hello m;
  if (Trap err = ch_.recv.pop_msg(h, m))
    return err;

  // Tentatively reject the connection if the version is not the
  // same. The handler may allocate a new state machine.
  if (h.version != VERSION)
    return {make_error_code(std::errc::protocol_not_supported), h.version};
  else
    return {};
}

/// Receiving a timeout now results in protocol termination.
Error
Machine::on_hello_wait_timeout() {
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
Machine::on_feature_wait_message(const Header& h) {
  if (h.type != FEATURE_REPLY)
    return make_error_code(std::errc::protocol_error);

  Feature_reply m;
  if (Trap err = ch_.recv.pop_msg(h, m))
    return err;

  // FIXME: Enter a feature discovery phase? Probbly not necesssary.
  //
  // FIXME: Actually build or cnfigure the switch object.

  return enter_established();
}

/// Receiving a timeout now results in protocol termination.
Error
Machine::on_feature_wait_timeout() {
  return make_error_code(std::errc::timed_out);
}


// -------------------------------------------------------------------------- //
// Processing state transitions

// FIXME: Do something here.
Error
Machine::on_established_message(const Header& h) {
  return {};
}


} // namespace v1_0
} // namespace ofp
} // namespace nocontrol
