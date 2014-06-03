// Copyright (c) 2013-2014 Flowgrammable, LLC.
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

#ifndef FREEFLOW_LISTENER_HPP
#define FREEFLOW_LISTENER_HPP

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/acceptor.hpp>
#include <freeflow/sys/reactor.hpp>

namespace freeflow {

class Controller;

/// The Default_listener_factory is responsible for the allocation of
/// Services for a given controller.
template<typename Service>
  struct Default_listener_factory {
    Default_listener_factory(Controller&);

    Service* operator()(Reactor&, Socket&&) const;

  private:
    Controller& ctrl_;
  };

/// A listener is an acceptor bound to a controller. This class is
/// primarily provided as a convenience, providing an appropriate
/// default factory and useful constructor.
///
/// \todo Consider making this an actual component of the controller
/// rather than just a trivial wrapper.
template<typename Service, typename Factory = Default_listener_factory<Service>>
  struct Listener : Acceptor<Service, Factory> {
    Listener(Controller& ctrl);
  };

} // namespace freeflow

#include <freeflow/sdn/listener.ipp>

#endif

