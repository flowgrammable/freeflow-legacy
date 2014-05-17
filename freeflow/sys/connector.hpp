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

/// \todo This is identical to Default_acceptor. Unify these classes.
template<typename Svc>
  struct Default_connector {
    Svc* operator()(Reactor&, Socket&&) const;
  };

/// The Connector class is an instance of the connector design pattern.
/// Its purpose is to decouple the connection processor from the protocol 
/// or service implementation. When a connection is made, the connecting
/// socket is transferred to a new service handler, and the connector
/// is terminated.
///
/// The Con parameter provides the actual constructor for the connected 
/// service. This allows users to inject additional information into the 
/// created service when the connection is accepted.
template<typename Svc, typename Con = Default_connector<Svc>>
  class Connector : public Socket_handler {
  public:
    template<typename... Args>
      Connector(Reactor&, const Address&, Socket::Transport, Args&&...);

    // Events
    bool on_write();

  private:
    Con factory_;
  };

} // namespace freeflow

#include <freeflow/sys/connector.ipp>

#endif
