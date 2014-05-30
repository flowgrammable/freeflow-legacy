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

#ifndef NOCONTROL_OFP_HANDLER_HPP
#define NOCONTROL_OFP_HANDLER_HPP

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/handler.hpp>
#include <freeflow/sys/acceptor.hpp>
#include <freeflow/sdn/controller.hpp>
#include <freeflow/proto/ofp/protocol.hpp>

#include "prelude.hpp"

namespace nocontrol {

/// Represents an OpenFlow connection with a switch. This class acts as a 
/// small shim that buffers messages and hands them to a state machine for
/// further processing.
///
/// \todo We're required to TLS for switch connections. This
/// seems like the likely integration point for that work.
class Ofp_handler : public ff::Socket_handler {
  struct Write_on_exit;
public:
  Ofp_handler(ff::Reactor&, ff::Socket&&, ff::Controller&);

  bool on_open();
  bool on_close();
  bool on_read();
  bool on_time(int);

private:
  bool read();
  bool write();

  ff::Controller& ctrl_;
  ff::ofp::Protocol* proto_;
};

/// The Ofp_acceptor is responsible for accepting connections from
/// switches and constructing service handlers to manage the OpenFlow
/// protocol.
using Ofp_listener = ff::Listener<Ofp_handler>;

} // namespace nocontrol

#include "openflow.ipp"

#endif
