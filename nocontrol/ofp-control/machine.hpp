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
  enum State { INITIAL, NEGOTIATE, DISCOVER, PROCESS, FINAL};

  Machine(Channel&, Controller*);

  // Messaging infrastructure
  template<typename T>
    ff::Error push_msg(const T&);

  Uint32 xid();

  // State queries
  State state() const;
  bool in_negotiation() const;
  bool in_discovery() const;
  bool in_processing() const;

  // State entry events
  Error on_initial();
  Error on_negotiate(int);
  Error on_reject();
  Error on_discover();
  Error on_run();
  Error on_close();
  Error on_final();


  // State transition events
  Error on_message();
  Error on_initial_message(const Header&);
  Error on_initial_hello(const Header&);
  Error on_initial_other();
  Error on_initial_timeout();

  // Asynchronous messagess
  Error send_hello(const Buffer& = {});
  
  Error send_error(Error_message::Hello_failed);
  Error send_error(Error_message::Bad_request);
  Error send_error(Error_message::Bad_action);
  Error send_error(Error_message::Flow_mod_failed);
  Error send_error(Error_message::Port_mod_failed);
  Error send_error(Error_message::Queue_op_failed);
  
  Error send_feature_request();

  /// FIXME: These should all be part of the ofp errc enumeration.
  Error send_vendor();

  // Acknowledged requests
  void request_echo();
  void request_features();
  void request_ip_config();
  void request_queue_config();
  void request_stats();
  void request_barrier();

  // State-modifying messages.
  void set_config();
  
  void send_packet();
  void release_packet();
  void modify_flow();
  void remove_flow();
  void modify_port();

  // Events
  void on_packet();
  void on_flow_removed();
  void on_port_status();

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
