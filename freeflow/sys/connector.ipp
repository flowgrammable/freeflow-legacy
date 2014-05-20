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

/// \todo Improve error handling.
template<typename S>
  inline
  Connector<S>::Connector(Reactor& r)
    : Socket_handler(r, WRITE_EVENTS) , eh_(nullptr)
  { }

/// Initiate an asynchronous connection. When connection occurs, transfer
/// event handling responsibility to the given handler. 
///
/// \todo Allow multiple asynchronous connections to be initiated at
/// the same time? To do that, this would basically have to maintain
/// a set of sub-event handlers, each binding the created socket
/// to the corresponding event handler.
///
/// \todo Improve error handling.
template<typename S>
  inline void
  Connector<S>::connect(S* h, const Address& a, Transport t) {
    assert(not rc());
    assert(not eh_);

    // Save the event handler.
    eh_ = h;

    // Initialize the socket. This will throw if there is an error.
    assign(Socket(a.family(), t));
    rc().set_nonblocking();

    // Initiate the asynchronous connection.
    System_result res = rc().connect(a);
    if (res.failed())
      throw std::runtime_error(strerror(errno));

    // Return the socket to a blocking state for subsequent operations.
    rc().set_blocking();

    // Register the connector with the handler so that it can 
    // receive the connection notification.
    reactor().add_handler(this);
  }

/// Try to establish a connection to the given address using the specified
/// transport protocol. After connection has completed, the event handler
/// is added to the connector's reactor.
///
/// Note that immediate connection does not rely on the reactor to determine
/// if the connection has completed.
template<typename S>
  inline void
  Connector<S>::connect_now(S* h, const Address& a, Transport t) {
    // Initialize the socket and initiate the connection.
    Socket s(a.family(), t);
    System_result res = s.connect(a);
    if (res.failed())
      throw std::runtime_error(strerror(errno));

    h->assign(std::move(s));
    reactor().add_handler(h);
  }

/// Called when the connection has completed or failed. This is only called
/// if the connection was initiated in an asynchronous manner.
template<typename S>
  inline bool 
  Connector<S>::on_write() {
    // FIXME: Actually check that the connection succeeded!

    // Transfer control of the connector the given service.
    reactor().remove_handler(this);
    eh_->assign(std::move(rc()));
    reactor().add_handler(eh_);

    // Reset the connector.
    // FIXME: Is this sufficient for reset?
    eh_ = nullptr;
    return true;
  }

} // namespace freeflow
