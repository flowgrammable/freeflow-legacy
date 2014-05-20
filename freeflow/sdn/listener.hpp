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

/// The listener class is is the abstract base class of all Acceptor
/// instances listening for connections.
class Listener {
public:
  virtual ~Listener();

  Socket::Trnsport transport() const;
  const Address& addr() const;


protected:
  Socket* sock_ = nullptr;  // The listening address.
};


/// This class is a helper for listener implementaitons.
template<typename Service>
  struct Listener_base : Acceptor<Service>, Listener {
    Listener_base(Reactor& r, const Address&, Socket::Transport, int);
  };

} // namespace freeflow

#endif

