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

namespace nocontrol {

// A scope guard that guarantees messages are always written before
// returning from the function.
struct Connection::Write_on_exit {
  Write_on_exit(Connection& c) : conn(c) { }
  ~Write_on_exit() { conn.write(); }
  Connection& conn;
};

inline
Connection::Connection(ff::Socket&& s)
  : Resource_handler<ff::Socket>(std::move(s)) { }

/// Create a version negotiation state machine so we know what version
/// we should accept.
///
/// \todo Error checking.
inline bool
Connection::on_open(ff::Reactor& r) {
  Write_on_exit g(*this);
  proto_ = new ff::ofp::Protocol(this);
  return proto_->on_open(r);
}

/// Shutdown the state machine and delete the handler.
inline bool 
Connection::on_close(ff::Reactor& r) {
  Write_on_exit g(*this);
  proto_->on_close(r);
  delete this;
  return true; 
}

/// When data is available, read as many messages as possibly and send
/// them to the state machine.
///
/// \todo Error checking.
inline bool
Connection::on_read(ff::Reactor& r) {
  Write_on_exit g(*this);
  if (not read()) 
    return false;
  return proto_->on_recv(r);
}

/// When a timeout occurs, notify the protocol of the expired timer.
inline bool
Connection::on_time(ff::Reactor& r, int t) {
  Write_on_exit g(*this);
  return proto_->on_time(r, t);
}

} // namespace nocontrol
