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
template<typename H>
  inline
  Connector<H>::Connector(Reactor& r)
    : Socket_handler(r, WRITE_EVENTS) , eh_(nullptr)
  { }

/// Initiate an asynchronous connection. When connection occurs, transfer
/// event handling responsibility to the given handler. 
///
/// \todo Allow multiple asyncrhonous connections to be initiated at
/// the same time? To do that, this would basically have to maintain
/// a set of sub-event handlers, each binding the created socket
/// to the corresponding event handler.
///
/// \todo Improve error handling.
template<typename H>
  inline void
  Connector<H>::connect(const Address& a, Transport t, Event_handler* h) {
    assert(not rc());
    assert(not eh_);
    
    // Initialize the socket. This will throw if there is an error.
    rc() = Socket(a.family(), t);
    rc().set_nonblocking();

    // Initiate the asynchronous connection.
    System_result res = rc().connect();
    if (res.failed())
      throw std::runtime_error(strerror(errno));
  }

/// Called when the connection has completed or failed. 
template<typename H>
  inline bool 
  Connector<H>::on_write() {
    // FIXME: Actually check that the connection succeeded!

    // Transfer control of the connector the given service.
    eh_->rc() = std::move(rc());
    eh_->open();

    // Reset the connector.
    // FIXME: Is this sufficent for reset?
    eh_ = nullptr;
    return true;
  }

} // namespace freeflow
