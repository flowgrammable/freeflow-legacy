// Copyright (c) 2013-2014 Flowgrammable, LLC.
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

#ifndef FREEFLOW_SWITCH_HPP
#define FREEFLOW_SWITCH_HPP

#include <cassert>

#include <freeflow/sys/data.hpp>
#include <freeflow/nbi/application.hpp>
#include <freeflow/nbi/request.hpp>

namespace freeflow {

struct Socket;
class Controller;

/// A Switch represents a connected physical packet switching device.
///
/// \todo Allow the switch to run multiple applications.
class Switch {
public:
  Switch(Controller&, Socket& s);

  // Observers
  Controller& controller();

  // Transport
  // FIXME: Get socket address and other information.

  // Protocol
  Uint8 protocol_version() const;
  Uint8 protocol_experiment() const;
  bool set_protocol(Uint8, Uint8);

  // Features
  bool ready();
  
  // Application control
  bool bind(Application*);
  void unbind(Application*);
  void unbind();

  // Requests
  void disconnect();

private:
  Controller&   ctrl_;
  Socket&       sock_;
  Request_queue reqs_;
  
  // The hosted application.
  // TODO: Should be applications.
  Application* app_; 
  
  // TODO: Allow different protocols to be supported.
  Uint8 proto_vsn; // The negotiated protocol version.
  Uint8 proto_exp; // True if the protocol is experimental.
};

} // namespace freeflow

#include <freeflow/nbi/switch.ipp>

#endif

