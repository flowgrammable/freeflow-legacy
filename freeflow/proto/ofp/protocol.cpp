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

#include <freeflow/sys/socket.hpp>
#include <freeflow/nbi/controller.hpp>
#include <freeflow/nbi/switch.hpp>

#include <freeflow/proto/ofp/v1.0/message.hpp>
#include <freeflow/proto/ofp/v1.0/protocol.hpp>

#include "protocol.hpp"

namespace freeflow {
namespace ofp {

bool 
Protocol::on_open(Reactor& r) {
  // Extract the socket from the handler...
  // FIXME: This isn't pretty.
  using Socket_handler = Resource_handler<Socket>;
  Socket_handler* sock = dynamic_cast<Socket_handler*>(handler_);

  // Connect the switch.
  switch_ = &ctrl_->connect(sock->rc());
  return open_to_hello(r); 
}

bool
Protocol::on_close(Reactor&) { 
  // Disconnect the switch.
  ctrl_->disconnect(*switch_);
  return true; 
}

bool
Protocol::on_recv(Reactor& r) {
  if (state_ == HELLO)
    return hello_recv(r);
  if (state_ == FEATURE)
    return feature_recv(r);
  if (state_ == ESTABLISHED)
    return established_recv(r);
  return true;
}

bool
Protocol::on_time(Reactor& r, int t) { 
  if (state_ == HELLO)
    return hello_recv(r);
  if (state_ == FEATURE)
    return feature_recv(r);
  if (state_ == ESTABLISHED)
    return established_time(r, t);
  return true;
}

bool
Protocol::open_to_hello(Reactor& r) {
  state_ = HELLO;

  // Send the hello message.
  // FIXME: Send hello from the highest protocol version supported.
  put_message(v1_0::Hello{});

  // Install a timeout timer.
  r.schedule_timer(handler_, ctime_, config_.connection_timeout);

  return true;
}

/// \todo What do we do if the hello has extra data? Presumably we'd pass
/// that to some kind of vendor extension?
bool
Protocol::hello_recv(Reactor& r) {
  Header h;
  read.peek_header(h);
  if (h.type != v1_0::HELLO)
    return false;

  // Negotiate the current protocol version.
  proto_ = negotiate(h.version);
  if (not proto_)
    return hello_to_close(r);
  else
    return hello_to_feature(r);
}

/// A timeout in the hello state results in shutdown.
bool
Protocol::hello_time(Reactor& r) { return hello_to_close(r); }

/// Send a feature request.
bool 
Protocol::hello_to_feature(Reactor& r) {
  state_ = FEATURE;

  // Send a feature request message.
  // FIXME: Send a more appropriate message version.
  put_message(v1_0::Feature_request{});

  // Reschedule the timeout.
  r.reschedule_timer(handler_, ctime_, config_.connection_timeout);

  return true;
}

bool 
Protocol::hello_to_close(Reactor& r) { return false; }

bool
Protocol::feature_recv(Reactor& r) {
  // Reschedule the ping timer. 
  r.reschedule_timer(handler_, mtime_, config_.message_timeout);

  // Determine the kind of message.
  // FIXME: Check against the negotiated version, not 1.0.
  Header h;
  read.peek_header(h);
  if (h.version != v1_0::VERSION)
    return false;
  if (h.type != v1_0::FEATURE_REPLY)
    return false;

  // FXIME: Actually interpret the payload.
  v1_0::Feature_reply p;
  get_payload(h, p);

  return feature_to_established(r);
}

/// Receiving a timeout in the feature state results
/// in shutdown.
bool
Protocol::feature_time(Reactor& r) { return feature_to_close(r); }

/// When moving the the established state, we only need to initiate
/// the echo timer.
bool
Protocol::feature_to_established(Reactor& r) {
  state_ = ESTABLISHED;

  // Cancel the timeout timer.
  r.cancel_timer(handler_, ctime_);

  // Set the message timer to send a ping if we haven't heard
  // from the switch in a while.
  r.schedule_timer(handler_, mtime_, config_.message_timeout);

  return true;
}

bool
Protocol::feature_to_close(Reactor& r) { return false; }

bool
Protocol::established_recv(Reactor& r) {
  // Read and dispatch the message.
  //
  // FIXME: Dispatch according to version first.
  Header h;
  read.peek_header(h);
  switch (h.type) {
  case v1_0::ECHO_REPLY: return established_recv_echo(r);
  default: return true;
  }
  return true;
}

/// After receiving an echo reply, cancel the timeout and schedule the
/// next ping message.
bool
Protocol::established_recv_echo(Reactor& r) {
  r.cancel_timer(handler_, ctime_);
  return true;
}

/// Act on a timer.
bool
Protocol::established_time(Reactor& r, int t) {
  if (t == ctime_)
    return established_to_close(r);
  if (t == mtime_)
    return ping(r);
  return true;
}

bool
Protocol::established_to_close(Reactor&) { return false; }

/// Send an echo request and set the timer.
bool
Protocol::ping(Reactor& r) {
  v1_0::Echo_request m;
  put_message(m);
  r.schedule_timer(handler_, ctime_, config_.connection_timeout);
  return true;
}

// Configure the protocol version.
// Note that version negotiation should not fail.
//
// FIXME: We have to do some stuff with version bitmaps
// here... not sure what.
Protocol::Version*
Protocol::negotiate(Uint8 v) {
  version_ = std::min(config_.version, v);
  switch (version_) {
  case 0:
    return new v1_0::Handler(*this);
  default:
    return nullptr; // unreachable
  }
}


} // namespace ofp
} // namespace freeflow