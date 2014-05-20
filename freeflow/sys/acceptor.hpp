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

#ifndef FREEFLOW_ACCEPTOR_HPP
#define FREEFLOW_ACCEPTOR_HPP

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/handler.hpp>

namespace freeflow {

template<typename Service>
  struct Default_accept_factory {
    Service* operator()(Reactor&, Socket&&) const;
  };

/// The acceptor class is an instance of the acceptor design pattern.
/// Its purpose is to decouple the acceptance of connections from the
/// protocol or service implementation. When a connection is accepted,
/// a new service of type Service is constructed. 
///
/// Note that the event handlers of accepted connection are managed
/// by the reactor.
///
/// The Factory parameter provides the actual constructor for the
/// accepted service. This allows users to inject additional informatoin
/// into the created service when the connection is accepted.
template<typename Service, typename Factory = Default_accept_factory<Service>>
  class Acceptor : public Socket_handler {
  public:
    using Transport = Socket::Transport;
    
    template<typename... Args>
      explicit Acceptor(Reactor&,  Args&&...);

    // Listen
    void listen(const Address&, Transport, int = SOMAXCONN);

    // Events
    bool on_read();

  private:
    Factory factory_;
  };

} // namespace freeflow

#include <freeflow/sys/acceptor.ipp>

#endif
