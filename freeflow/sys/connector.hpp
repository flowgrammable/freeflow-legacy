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

#include <vector>

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/handler.hpp>
#include <freeflow/sys/reactor.hpp>

namespace freeflow {

/// The connection handler class is a helper class for the Connector
/// pattern. It receives the initial write notification when the
/// connection is accepted, and it spins off a handler of the specified
/// type to manage the connection.
template<typename Handler, typename Factory>
  class Connection_handler : public Socket_handler {
  public:
    using Transport = Socket::Transport;
    
    Connection_handler(Reactor&, Factory&, const Address&, Transport);

    bool on_close() override;
    bool on_read() override;
    bool on_write() override;
    bool on_except() override;

  private:
    bool succeeded();
    bool failed();

  private:
    Factory& factory_;
    bool connect_;
  };

/// The Connector class is an instance of the connector design pattern.
/// Its purpose is to decouple the connection processor from the protocol 
/// or service implementation. When a connection is made, the connecting
/// socket is transferred to a new service handler, and the connector
/// is terminated.
template<typename Handler, typename Factory = Default_handler_factory<Handler>>
  class Connector {
    using Impl = Connection_handler<Handler, Factory>;
  public:
    using Transport = Socket::Transport;
    
    template<typename... Args>
      Connector(Reactor&, Args&&...);

    // Connection
    bool connect(const Address&, Transport);

  private:
    Reactor& reactor_;
    Factory  factory_;
  };

} // namespace freeflow

#include <freeflow/sys/connector.ipp>

#endif
