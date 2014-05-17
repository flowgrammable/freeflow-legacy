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

template<typename Svc>
  struct Default_acceptor {
    Svc* operator()(Reactor&, Socket&&) const;
  };

/// The acceptor class is an instance of the acceptor design pattern.
/// Its purpose is to decouple the acceptance of connections from the
/// protocol or service implementation. When a connection is accepted,
/// a new service of type Svc is constructed.
///
/// The Acc parameter provides the actual constructor for the
/// accepted service. This allows users to inject additional informatoin
/// into the created service when the connection is accepted.
template<typename Svc, typename Acc = Default_acceptor<Svc>>
  class Acceptor : public Socket_handler {
  public:
    template<typename... Args>
      Acceptor(Reactor&, const Address&, Socket::Transport, Args&&...);

    // Events
    bool on_read();

  private:
    Acc acceptor_;
  };

} // namespace freeflow

#include <freeflow/sys/acceptor.ipp>

#endif
