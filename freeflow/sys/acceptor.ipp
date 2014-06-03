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

/// Initialize the acceptor to work with the given reactor. The 
/// additional arguments are forwraded to the accept factory.
template<typename S, typename F>
  template<typename... Args>
    inline
    Acceptor<S, F>::Acceptor(Reactor& r, Args&&... args)
      : Socket_handler(r, READ_EVENTS), factory_(std::forward<Args>(args)...)
    { }

/// Start listening for connections on the given address.
///
/// \todo Improve error handling.
///
/// \todo Should this auto-register the handler? Maybe...
template<typename S, typename F>
  inline void
  Acceptor<S, F>::listen(const Address& a, Transport t, int backlog) {
    // Re-initialize the socket.
    assign(Socket(a.family(), t));

    // Bind to the given address and listen for connections.
    if (Trap err = rc().bind(a))
      throw System_error(err.code());
    if (Trap err = rc().listen(backlog))
      throw System_error(err.code());
  }

/// Called when a connection is available. This invokes the acceptor
/// factory to create a new event handler, which is registerd with
/// the acceptor's reactor.
///
/// \todo Error handling. If there's a socket error, we may have to
/// close and reopen the socket.
template<typename S, typename F>
  inline bool 
  Acceptor<S, F>::on_read() {
    Socket s = accept(rc());
    S* h = factory_(reactor(), std::move(s));
    if (h)
      reactor().new_handler(h);
    return true;
  }

} // namespace freeflow
