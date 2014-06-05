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

#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <freeflow/proto/ofp/v1.0/message.hpp>

#include "prelude.hpp"
#include "queue.hpp"

namespace freeflow {
namespace ofp {
namespace v1_0 {

/// The v1.0 OFP state machine for controllers.
class Machine {
public:
  /// Records the current state.
  enum State { 
    CLOSED, HELLO_WAIT, NEGOTIATE, REJECT, FEATURE_WAIT, ESTABLISHED 
  };

  Machine(Channel&, Controller*);

  // Messaging infrastructure
  template<typename T>
    ff::Error push_msg(const T&);

  Uint32 xid();

  // State queries
  State state() const;
  bool in_negotiation() const;

  // State entry events
  Error enter_hello_wait();
  Error enter_version_negotiation();
  Error enter_version_rejection();
  Error enter_feature_wait();
  Error enter_established();

  // State transitions
  Error on_message();

private:
  // State transition events
  Error on_hello_wait_message(const Header&);
  Error on_hello_wait_timeout();
  Error on_feature_wait_message(const Header&);
  Error on_feature_wait_timeout();
  Error on_established_message(const Header&);

private:
  // Asynchronous messagess
  Error send_hello(const Buffer& = {});
  Error send_error(Error_message::Hello_failed, const Buffer& = {});
  Error send_error(Error_message::Bad_request, const Buffer& = {});
  Error send_error(Error_message::Bad_action, const Buffer& = {});
  Error send_error(Error_message::Flow_mod_failed, const Buffer& = {});
  Error send_error(Error_message::Port_mod_failed, const Buffer& = {});
  Error send_error(Error_message::Queue_op_failed, const Buffer& = {});
  Error send_vendor(const Buffer& = {});

  // Acknowledged requests
  Error request_echo();
  Error request_features();
  Error request_ip_config();
  Error request_queue_config();
  Error request_stats();
  Error request_barrier();

  // State-modifying messages.
  Error set_config();
  
  Error send_packet();
  Error release_packet();
  Error modify_flow();
  Error remove_flow();
  Error modify_port();

  // Events
  Error on_packet();
  Error on_flow_removed();
  Error on_port_status();

private:
  Channel&        ch_;    // Communicaitons channel
  State           state_; // Current state

  // SDN Elements
  Controller* ctrl_;  // The controller
};

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#include "machine.ipp"

#endif
