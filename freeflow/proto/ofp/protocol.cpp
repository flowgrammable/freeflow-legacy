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

#include <algorithm>

#include <freeflow/proto/ofp/v1.0/message.hpp>
#include <freeflow/proto/ofp/v1.0/protocol.hpp>

#include "protocol.hpp"

namespace freeflow {
namespace ofp {

bool 
Protocol::on_open(Reactor& r) { return open_to_hello(r); }

bool
Protocol::on_close(Reactor&) { return true; }

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
  r.schedule_timer(handler_, timer_, config_.echo_timeout);

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

  // Update the timer.
  r.reschedule_timer(handler_, timer_, config_.echo_timeout);

  return true;
}

bool 
Protocol::hello_to_close(Reactor& r) { return false; }

bool
Protocol::feature_recv(Reactor& r) {
  Header h;
  read.peek_header(h);

  // FIXME: Check against the negotiated version, not 1.0.
  if (h.version != v1_0::VERSION)
    return false;
  if (h.type != v1_0::FEATURE_REPLY)
    return false;

  // FXIME: Read the right message! Also... do something with this.
  // but what?
  v1_0::Feature_reply p;
  get_payload(h, p);

  return feature_to_established(r);
}

bool
Protocol::feature_time(Reactor& r) { return feature_to_close(r); }

/// When moving the the established state, we only need to initiate
/// the echo timer.
bool
Protocol::feature_to_established(Reactor& r) {
  state_ = ESTABLISHED;


  // Go ahead and send the first ping now.
  v1_0::Echo_request m;
  put_message(m);

  // Schedule the echo timer.
  r.schedule_timer(handler_, echo_, config_.echo_timeout);

  // Cancel the timeout timer for now.
  r.cancel_timer(handler_, timer_);

  return true;
}

bool
Protocol::feature_to_close(Reactor& r) { return false; }

bool
Protocol::established_recv(Reactor& r) {
  Header h;
  read.peek_header(h);

  // FIXME: Dispatch according to version first.
  switch (h.type) {
  case v1_0::ECHO_REPLY: return established_recv_echo(r);
  }
  return true;
}

/// After receiving an echo reply, cancel the timeout and schedule the
/// next ping message.
bool
Protocol::established_recv_echo(Reactor& r) {
  r.cancel_timer(handler_, echo_);
  r.reschedule_timer(handler_, echo_, config_.echo_timeout / 2);
  return true;
}

// FIXME: There are lots of potential timers here.
bool
Protocol::established_time(Reactor& r, int t) {
  if (t == timer_)
    return established_to_close(r);
  if (t == echo_)
    ping(r);
  return true;
}

bool
Protocol::established_to_close(Reactor&) { return false; }

/// Send an echo request and set the timer.
bool
Protocol::ping(Reactor& r) {
  v1_0::Echo_request m;
  put_message(m);
  r.schedule_timer(handler_, timer_, config_.echo_timeout);
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