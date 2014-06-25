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

#ifndef FREEFLOW_OFP_PROTOCOL_HPP
#define FREEFLOW_OFP_PROTOCOL_HPP

#include <queue>

#include <freeflow/sys/buffer.hpp>
#include <freeflow/sys/time.hpp>
#include <freeflow/sys/reactor.hpp>

#include <freeflow/sdn/switch.hpp>
#include <freeflow/sdn/request.hpp>

#include <freeflow/proto/ofp/ofp.hpp>

namespace freeflow {

// class Controller;
// class Switch;

namespace ofp {

/// A message queue represents a sequence of buffered messages
/// with each buffer containing a single message. 
///
/// There are parts of the interface: get and put buffer are used by the
/// lower level to move data to the protocol state machine. The get
/// and put message operations are used by the commications protocol
/// to move data in the reverse direction.
///
/// \todo The message queue could be made protocol dependent, with the
/// message interface being the only facility in this namespace.
///
/// \todo Implement error handling.
struct Message_queue : std::queue<Buffer> {

  void put_buffer(Buffer&&);
  void get_buffer(Buffer&);

  template<typename H, typename P>
    Error put_message(const H&, const P&);

  template<typename H>
    Error peek_header(H&);

  template<typename H, typename P>
    Error get_payload(const H&, P&);
};


/// The Config class represents that runtime configuration of
/// OpenFlow support.
///
/// \todo This needs to be read from a configuration file and
/// populated dynamically.
struct Config {
  /// The highest version of the protocol supported.
  Uint8 current_version = 1;

  /// If, after this duration, the controller has not received any 
  /// messages from a connected switch, send an echo request to determine
  /// if the connection is still live.
  Seconds message_timeout = 10_s;

  /// The amount of time to wait before determining that a switch
  /// is no longer connected. This duration governs timeout policies
  /// for both handshake messages and echo requests.
  Seconds connection_timeout = 60_s;

  /// The default idle timeout for flows.
  Seconds idle_timeout = 5_s;

  /// The default hard timeout for flows.
  Seconds hard_timeout = 10_s;
};


// The Protocol_handler class represents the base class of
// state machines for specific versions of the protocol.
//
// Technically, this is a decoarator.
//
// \todo I'm not sure I actually like this design.
struct Protocol_handler {
  virtual void to_feature() = 0;
  virtual void wait_feature() = 0;
};


/// The Protocol represents phases of a more general protocol.
/// OpenFlow defines a number of distinct protocols, each exchanging
/// different messages. Each version of the standard defines a different
/// protocol. Version negotation is common to all of them.
///
/// The primary mechanism for communicating with the communications
/// controller is a pair of message queues for reading and writing.
///
/// \todo Figure out how the controller and the reactor actually
/// fit together. Does the controller own the reactor?
class Protocol {
  enum State {
    CLOSED,
    HELLO,       // Version negotiation
    FEATURE,     // Feature discovery
    ESTABLISHED, // Basic processing
  };

public:
  Protocol(Controller*, Event_handler*);

  // Network events
  bool on_open(Reactor&);
  bool on_close(Reactor&);
  bool on_recv(Reactor&);
  bool on_time(Reactor&, int);

  // Application requests


  // Read/write helper functions
  //
  // FIXME: Should be private?
  template<typename P>
    Error put_message(const P& p);

  template<typename H>
    Error peek_header(H& h);

  template<typename H, typename P>
    Error get_payload(const H&, P& p);

  // Message queue
  Message_queue read;
  Message_queue write;

private:
  Uint32 xid();
  Protocol_handler* negotiate(Uint8);
  bool ping(Reactor&);

  // Open state and transitions
  bool open_to_hello(Reactor&);
  
  // Initial state and transitions
  bool hello_recv(Reactor&);
  bool hello_time(Reactor&);
  bool hello_to_feature(Reactor&);
  bool hello_to_close(Reactor&);
  
  // Discover state and transitions
  bool feature_recv(Reactor&);
  bool feature_time(Reactor&);
  bool feature_to_established(Reactor&);
  bool feature_to_close(Reactor&);

  // Established state and transitions
  bool established_recv(Reactor&);
  bool established_recv_echo(Reactor&);
  bool established_time(Reactor&, int);
  bool established_to_close(Reactor&);

  // Request processing
  bool service(Reactor&);
  bool service(Reactor&, const Request&);
  bool on_disconnect(Reactor&, const Request&);
  bool on_terminate(Reactor&, const Request&);


  // Internal processing facilities
  Event_handler*    handler_;
  Protocol_handler* proto_;
  Config            config_;
  State             state_;

  Uint32   xid_;       // The curent transaction id
  int      ctime_ = 0; // The connection timeout timer
  int      mtime_ = 1; // The message timeout timer

  // NBI features
  Controller* ctrl_;    // The controller hosting the state machine
  Switch*     switch_;  // The connected switch
};

} // ofp
} // freeflow

#include "protocol.ipp"

#endif
