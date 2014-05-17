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
  Default_connector<S>::operator()(Reactor& r, Socket&& s) const {
    return new S(r, std::move(s));
  }

/// \todo Improve error handling.
template<typename S, typename A>
  template<typename... Args>
    inline
    Connector<S, A>::Connector(Reactor& r, 
                               const Address& a,
                               Socket::Transport t,
                               Args&&... args)
      : Socket_handler(r, WRITE_EVENTS, a.family(), t)
      , factory_(std::forward<Args>(args)...) 
    {
      rc.set_nonblocking();
      System_result res = connect(rc(), a);
      if (res.failed())
        throw std::runtime_error("connection");
    }

/// Called when the connection has completed or failed. This creates
/// a new service, transferring the connected to socket to the
/// newly created service. The connector is removed from its reactor,
/// preventing any new messages from arriving.
template<typename S, typename A>
  inline bool 
  Connector<S, A>::on_write() {
    // Create the new srive 
    S* h = factory_(reactor(), std::move(rc()));
    if (h)
      reactor().new_handler(h);

    // This handler is no longer active after connection.
    reactor().remove_handler(this);
    return true;
  }

} // namespace freeflow
