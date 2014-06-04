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

namespace impl {
// Put the socket into non-blocking mode for the lifetime of this object.
// The socket is returned to blocking mode when the objects lifetime ends.
struct Nonblocking_socket_guard {
  Nonblocking_socket_guard(Socket& s) : s(s) { s.set_nonblocking(); }
  ~Nonblocking_socket_guard() { s.set_blocking(); }
  Socket& s;
};
} // namespace impl

// -------------------------------------------------------------------------- //
// Connection handler

/// On initialization, initiate an asynchronous connection request.
/// If an error occurs, an exception is thrown.
///
/// Note that apparentl;y a non-blocking connect receive notifications on 
/// read, write, and except channels. The connection is valid iff a
/// write notification is signaled first.
template<typename H, typename F>
  inline
  Connection_handler<H, F>::Connection_handler(Reactor& r, 
                                               F& f,
                                               const Address& a,
                                               Transport t)
    : Socket_handler(r, READ_EVENTS | WRITE_EVENTS | EXCEPT_EVENTS, a.family(), t)
    , factory_(f), connect_(false)
  { 
    impl::Nonblocking_socket_guard g(rc()); // Enter non-blocking mode

    // Inititate an asynchronous connect.
    // FIXME: Error handling.
    System_result res = rc().connect(a);
    if (res.failed())
      throw std::runtime_error(strerror(errno));
  }

/// The close handler is called when 
template<typename H, typename F>
  inline bool
  Connection_handler<H, F>::on_close() {
    // Create the new handler and register it with the reactor.
    H* h = factory_(reactor(), std::move(rc()));
    reactor().new_handler(h);

    // Delete the intermediate event handler.
    delete this;    
    return true;
  }

/// Received when an error occurs during connection. This returns
/// false, causing the handler to be removed from the reactor.
template<typename H, typename F>
  inline bool
  Connection_handler<H, F>::on_read() { return failed(); }

/// Received when a a connection is ready. This returns false, causing 
/// the handler to be removed from the reactor. Note that the actual
///
/// \todo Check the SO_ERROR socket option for failure. Note that this
/// may not be portable.
template<typename H, typename F>
  inline bool
  Connection_handler<H, F>::on_write() { return succeeded(); }

/// Received when an error occurs during connection. This returns
/// false, causing the handler to be removed from the reactor.
template<typename H, typename F>
  inline bool
  Connection_handler<H, F>::on_except() { return failed(); }


// Sets the connection state to true and returns false.
template<typename H, typename F>
  inline bool
  Connection_handler<H, F>::succeeded() { return not (connect_ = true); }

// Sets the connection state to false and returns false.
template<typename H, typename F>
  inline bool
  Connection_handler<H, F>::failed() { return (connect_ = false); }


// -------------------------------------------------------------------------- //
// Connector

/// Initialize the connector
template<typename H, typename F>
  template<typename... Args>
    Connector<H, F>::Connector(Reactor& r, Args&&... args)
      : reactor_(r), factory_(std::forward<Args>(args)...) { }

/// Initiate an asynchronous connection. When connection occurs, transfer
/// event handling responsibility to the given handler. 
///
/// \todo Allow multiple asynchronous connections to be initiated at
/// the same time? To do that, this would basically have to maintain
/// a set of sub-event handlers, each binding the created socket
/// to the corresponding event handler.
///
/// \todo Improve error handling.
template<typename H, typename F>
  inline bool
  Connector<H, F>::connect(const Address& a, Transport t) {
    // Create the handler, initiating the non-blocking connect.
    Impl* h = nullptr;
    try {
      h = new Impl(reactor_, factory_, a, t);
    } catch(...) {
      delete h;
      return false;
    }

    // Add the handler to the reactor. Note that the handler will
    // manage its own deletion.
    reactor_.add_handler(h);
    return true;
  }

} // namespace freeflow
