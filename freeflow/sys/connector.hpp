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

#ifndef FREEFLOW_CONNECTOR_HPP
#define FREEFLOW_CONNECTOR_HPP

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/handler.hpp>

namespace freeflow {

/// The Connector class is an instance of the connector design pattern.
/// Its purpose is to decouple the connection processor from the protocol 
/// or service implementation. When a connection is made, the connecting
/// socket is transferred to a new service handler, and the connector
/// is terminated.
template<typename Handler>
  class Connector : public Socket_handler {
  public:
    using Transport = Socket::Transport;

    Connector(Reactor&);

    // Connection
    void connect(const Address&, Transport, Event_handler*);

    // Events
    bool on_write();

  private:
    Handler* eh_;
  };

} // namespace freeflow

#include <freeflow/sys/connector.ipp>

#endif
