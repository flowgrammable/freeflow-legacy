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

#ifndef NOCONTROL_NOCONTROL_HPP
#define NOCONTROL_NOCONTROL_HPP

#include <freeflow/sys/acceptor.hpp>
#include <freeflow/sdn/controller.hpp>

#include "prelude.hpp"

namespace nocontrol {

/// NOTE: The abbreviation NCP means "NoControl Protocol".

/// The Ncp_handler is responsible for implementing queries and commands 
/// sent to this service. When data is available for reading, it reads 
/// the value, parses it into JSON, and dispatches the appropriate commands.
struct Ncp_handler : ff::Socket_handler {
  inline Ncp_handler(ff::Reactor&, ff::Socket&&, ff::Controller&);

  bool on_read();

private:
  ff::Controller& ctrl_;  // The controller
  ff::Buffer buf_;        // A pre-allocated read buffer.
};

/// The Ncp_acceptor is resposnble for accepting connections
/// on the management port and construcitng service handlers to
/// manage that connection.
using Ncp_listener = ff::Listener<Ncp_handler>;

} // namesapce nocontrol

#include "nocontrol.ipp"

#endif
