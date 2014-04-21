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

#ifndef NOCONTROL_CONNECTION_HPP
#define NOCONTROL_CONNECTION_HPP

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/handler.hpp>
#include <freeflow/proto/ofp/protocol.hpp>

#include <nocontrol/config.hpp>

namespace nocontrol {

/// Represents a connection to a switch. This class acts as a small
/// shim that buffers messages and hands them to a state machine for
/// further processing.
///
/// \todo We're required to TLS for switch connections. This
/// seems like the likely integration point for that work.
class Connection : public ff::Resource_handler<ff::Socket> {
  struct Write_on_exit;
public:
  using ff::Resource_handler<ff::Socket>::Resource_handler;

  Connection(ff::Controller&, ff::Socket&&);

  bool on_open(ff::Reactor&);
  bool on_close(ff::Reactor&);
  bool on_read(ff::Reactor&);
  bool on_time(ff::Reactor&, int);

private:
  bool read();
  bool write();

  ff::Controller* ctrl_;
  ff::ofp::Protocol* proto_;
};

} // namespace nocontrol

#include <nocontrol/connection.ipp>

#endif
