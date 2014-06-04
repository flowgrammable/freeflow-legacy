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
struct Machine {
  enum State { INIT, RUN };

  Machine(Channel&, Controller*);

  // Messaging infrastructure
  template<typename T>
    void push_msg(const T&);

  Uint32 xid();

  // State machine
  bool on_init();
  bool on_run();
  bool on_close();

  // Events
  bool on_message();

  // Asynchronous messagess
  Error send_hello(const Buffer& = {});
  Error send_error(Error::Hello_failed);
  Error send_error(Error::Bad_request);
  Error send_error(Error::Bad_action);
  Error send_error(Error::Flow_mod_failed);
  Error send_error(Error::Port_mod_failed);
  Error send_error(Error::Queue_op_failed);
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
