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

#ifndef NOCONTROL_ACCEPTOR_HPP
#define NOCONTROL_ACCEPTOR_HPP

#include <freeflow/sys/socket.hpp>

#include <nocontrol/handler.hpp>
#include <nocontrol/reactor.hpp>

namespace nocontrol {

// The acceptor is responsible for accepting connections when
// they are available.
struct Acceptor : Resource_handler<ff::Socket> {
  Acceptor(const ff::Address& a);

  // Events
  bool on_read(Reactor&);
};

} // namesapce nocontrol

#include <nocontrol/acceptor.ipp>

#endif
