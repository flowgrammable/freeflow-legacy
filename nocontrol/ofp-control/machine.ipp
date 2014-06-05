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

namespace freeflow {
namespace ofp {
namespace v1_0 {

/// The v1.0 OFP state machine for controllers.
inline
Machine::Machine(Channel& ch, Controller* ctrl)
  : ch_(ch), state_(INITIAL), ctrl_(ctrl) { }


/// Returns a new transaction id for the message.
///
/// FIXME: This is totally broken.
inline Uint32
Machine::xid() { return 0; }

template<typename T>
  inline Error
  Machine::push_msg(const T& x) {
    Header h { T::Kind, Uint16(bytes(h) + bytes(x)), xid() };
    return ch_.send.push_msg(h, x);
  }

/// Returns the current state of the machine,
inline Machine::State
Machine::state() const { return state_; }

/// Returns true if the state machine is negotiaing the protocol version.
inline bool
Machine::in_negotiation() const { return state_ == NEGOTIATE; }

/// Returns true if the state machine is discovering features.
inline bool
Machine::in_discovery() const { return state_ == DISCOVER; }

/// Returns true if the state mahcine is processing events.
inline bool
Machine::in_processing() const { return state_ == PROCESS; }


inline Error
Machine::send_hello(const Buffer& data) {
  return push_msg(Hello {data});
}

inline Error
Machine::send_feature_request() {
  return { };
}

inline Error
Machine::send_error(Error_message::Hello_failed c) {
  return push_msg(Error_message {Error_message::HELLO_FAILED, c});
}

inline Error
Machine::send_error(Error_message::Bad_request c) {
  return push_msg(Error_message {Error_message::BAD_REQUEST, c});
}

inline Error
Machine::send_error(Error_message::Bad_action c) {
  return push_msg(Error_message {Error_message::BAD_ACTION, c});
}

inline Error
Machine::send_error(Error_message::Flow_mod_failed c) {
  return push_msg(Error_message {Error_message::FLOW_MOD_FAILED, c});
}

inline Error
Machine::send_error(Error_message::Port_mod_failed c) {
  return push_msg(Error_message {Error_message::PORT_MOD_FAILED, c});
}

inline Error
Machine::send_error(Error_message::Queue_op_failed c) {
  return push_msg(Error_message {Error_message::QUEUE_OP_FAILED, c});
}


} // namespace v1_0
} // namespace ofp
} // namespace nocontrol
