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

#ifndef FREEFLOW_OPENFLOW_HPP
#define FREEFLOW_OPENFLOW_HPP

#include <freeflow/sdn/listener.hpp>

namespace freeflow {

/// The Ofp_switch class represents a connected OpenFlow switch and
/// is responsible for transacting various versions of the OpenFlow
/// protocol.
class Ofp_switch : Socket_handler {

};

/// The Ofp_listener accepts sockets on a particular port and creates
/// connection instances.
class Ofp_listener : Listener_base<Ofp_switch> {
  Ofp_listener(const Address&, Socket::Transport, int);
};

} // namespace freeflow

#include <freeflow/sdn/openflow.ipp>

#endif

