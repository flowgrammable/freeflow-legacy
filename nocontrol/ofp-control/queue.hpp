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

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <queue>

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>
#include <freeflow/sdn/switch.hpp>
#include <freeflow/sdn/request.hpp>

#include <freeflow/proto/ofp/ofp.hpp>

namespace freeflow {
namespace ofp {

/// The Messgae_queue class represents the communication mechanism between
/// an event handler and an OpenFlow state machine. The queue contains
/// a sequence of buffers, each represneting a single message.
///
/// When a message is received by the event handler, the buffer is pushed
/// into the queue using push_buf, and the state machine is notified of data
/// availability. The state machine peeks at the message header using
/// peek_msg() in order to determine message viability and type. It is
/// eventually read and removed from the queue using one of the pop_msg()
/// overloads. The state machine pushes response messages into the queue
/// using the push_msg() function, and that messages is removed by the
/// event handler using pop_buf().
///
/// \todo Implement error handling.
///
/// \todo Movable? Copyable?
class Message_queue : private std::queue<Buffer> {
  using Base = std::queue<Buffer>;
public:
  // Imported base class operations
  using Base::empty;

  // Event-handler interface
  void push_buf(Buffer&&);
  void pop_buf(Buffer&);

  // State machine interface
  template<typename H, typename P>
    Error push_msg(const H&, const P&);

  template<typename H>
    Error peek_msg(H&);

  template<typename H, typename P>
    Error pop_msg(const H&, P&);
};


/// A Message channel is a pair of queues, facilitating communication
/// between an event handler and a state machine. The recv queue is used
/// to send messages from the event handler to the state machine. The
/// sene queue is used to send messages from the state machine to the
/// event handler (and the socket).
struct Channel {
  Message_queue recv;
  Message_queue send;
};

} // namespace ofp
} // namespace freeflow

#include "queue.ipp"

#endif
