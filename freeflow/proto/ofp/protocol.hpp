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

#include <list>

#include <freeflow/sys/buffer.hpp>

namespace freeflow {
namespace ofp {

// A message queue represents a sequence of buffered messages
// with each buffer containing a single message. 
//
// There are parts of the interface: get and put buffer are used by the
// lower level to move data to the protocol state machine. The get
// and put message operations are used by the commications protocol
// to move data in the reverse direction.
//
// TODO: The message queue could be made protocol dependent, with the
// message interface being the only facility in this namespace.
//
// FIXME: Error handling.
struct Message_queue : std::list<Buffer> {
  void put_buffer(Buffer&& buf) {
    push_back(std::move(buf));
  }

  void get_buffer(Buffer& buf) {
    buf = std::move(back());
    pop_back();
  }

  // Put a message into the queue.
  template<typename M>
    void put_message(const M& msg) {
      emplace_back(bytes(msg));
      View v(back());
      to_view(v, msg);
    }

  template<typename M>
    void get_message(M& msg) {
      View v(back());
      from_view(v, msg);
      pop_back();
    }
};

/// The Protocol represents phases of a more general protocol.
/// OpenFlow defines a number of distinct protocols, each exchanging
/// different messages. Each version of the standard defines a different
/// protocol. Version negotation is common to all of them.
///
/// The primary mechanism for communicating with the communications
/// controller is a pair of message queues for reading and writing.
/// Received messages are written 
///
/// TODO: These all take a time argument.
struct Protocol {
  void open();
  void close();
  void message();
  void time();

  Message_queue read;
  Message_queue write;

  int my_version = 0;   // Maximum supported version
  int neg_version = -1; // Negotiated protocol version (-1 is unspecified)
};

} // ofp
} // freeflow

#endif
