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

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/handler.hpp>
#include <freeflow/sys/reactor.hpp>
#include <freeflow/nbi/controller.hpp>

#include <nocontrol/config.hpp>

namespace nocontrol {

// The acceptor is responsible for accepting connections when
// they are available.
class Switch_acceptor : public ff::Resource_handler<ff::Socket> {
public:
  Switch_acceptor(ff::Controller&, const ff::Address&);

  // Events
  bool on_read(ff::Reactor&);

private:
  ff::Controller& ctrl_;
};

} // namesapce nocontrol

#include <nocontrol/switch_acceptor.ipp>

#endif