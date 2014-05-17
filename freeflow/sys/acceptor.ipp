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

namespace freeflow {

/// The default accept factor creates a new service that wraps the
/// accepted socket.
template<typename S>
  inline S*
  Default_acceptor<S>::operator()(Reactor& r, Socket&& s) const {
    return new S(r, std::move(s));
  }

/// \todo Provide a more reasonable backlog? Something configured
/// perhaps? This could be information supplied by the Acc policy.
template<typename S, typename A>
  template<typename... Args>
    inline
    Acceptor<S, A>::Acceptor(Reactor& r, 
                             const Address& a,
                             Socket::Transport t,
                             Args&&... args)
      : Socket_handler(r, READ_EVENTS, a.family(), t)
      , acceptor_(std::forward<Args>(args)...) 
    {
      rc().bind(a); 
      rc().listen();
    }

/// Called when a connection is available. This invokes the acceptor
/// factory to create a new event handler, which is registerd with
/// the acceptor's reactor.
///
/// \todo Error handling. If there's a socket error, we may have to
/// close and reopen the socket.
template<typename S, typename A>
  inline bool 
  Acceptor<S, A>::on_read() {
    Socket s = accept(rc());
    S* h = acceptor_(reactor(), std::move(s));
    if (h)
      reactor().new_handler(h);
    return true;
  }

} // namespace freeflow
