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

#ifndef NOCONTROL_CONTROL_ACCEPTOR_HPP
#define NOCONTROL_CONTROL_ACCEPTOR_HPP

#include <freeflow/sys/acceptor.hpp>
#include <freeflow/nbi/controller.hpp>

#include <nocontrol/config.hpp>

namespace nocontrol {

/// The Control_factory class is responsible for handling management
/// connections.
struct Control_factory {
  Control_factory(ff::Controller& c);

  ff::Event_handler* operator()(ff::Reactor& r, ff::Socket&& s);

  ff::Controller& ctrl;
};

/// The Control_acceptor is resposnble for accepting connections
/// on the management port and construcitng service handlers to
/// manage that connection.
///
/// \todo This should be called Mangagement_acceptor or something
/// like that.
using Control_acceptor = ff::Acceptor<ff::Event_handler, Control_factory>;

} // namesapce nocontrol

#include <nocontrol/control_acceptor.ipp>

#endif
