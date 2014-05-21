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

#ifndef NOCONTROL_SWITCH_ACCEPTOR_HPP
#define NOCONTROL_SWITCH_ACCEPTOR_HPP

#include <freeflow/sys/acceptor.hpp>
#include <freeflow/sdn/controller.hpp>

#include <nocontrol/config.hpp>
#include <nocontrol/connection.hpp>

namespace nocontrol {

/// The Switch_acceptor is responsible for creating new service handlers
/// for connected switches.
struct Switch_factory {
  Switch_factory(ff::Controller&);
  
  Connection* operator()(ff::Reactor&, ff::Socket&&);

  ff::Controller& ctrl;
};

/// The Switch_acceptor is responsible for accepting connections from
/// switches and constructing service handlers to manage the OpenFlow
/// protocol.
using Switch_acceptor = ff::Acceptor<Connection, Switch_factory>;

} // namesapce nocontrol

#include <nocontrol/switch_acceptor.ipp>

#endif
