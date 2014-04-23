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
  // FIXME: This isn't pretty. It should be.
  using Socket_handler = Resource_handler<Socket>;
  Socket_handler* sock = dynamic_cast<Socket_handler*>(handler_);

  // Connect the switch, sending a bind event to any associated
  // applications.
  switch_ = &ctrl_->connect(sock->rc());

  // Service any events resulting from the connect.
  if (not service(r))
    return false;

  return open_to_hello(r); 
}

/// When the connection is shutdown, release any switch-related
/// resources in the controller.
bool
Protocol::on_close(Reactor&) { 
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

  // Service any events resulting from version negotiation.
  if (not service(r))
    return false;

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
  if (protocol_version(h) != switch_->protocol_version())
    return false;
  if (h.type != v1_0::FEATURE_REPLY)
    return false;

  // FXIME: Actually update the switch with features and capabilities
  // read from the reply. There should be a sequence of calls to the
  // switch to initialize its configuration.
  v1_0::Feature_reply p;
  get_payload(h, p);

  // Inidate that the switch is done being configured.
  switch_->configured();

  // Service any requests resulting from feature discovery.
  if (not service(r))
    return false;

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
// Note that version negotiation should never fail, assuming that we've
// actually impletemeted the full set of protocols.
Protocol::Version*
Protocol::negotiate(Uint8 v) {
  Uint8 v0 = std::min(config_.current_version, v);
  Uint8 v1 = protocol_version(v0),
        ve = protocol_experiment(v1);

  // Set protocol the protocol information on the switch.
  switch_->set_protocol(v1, ve);

  switch (v) {
  case 0:
    return new v1_0::Handler(*this);
  default:
    return nullptr; // Not supported.
  }
}

// Service any outstanding requests by sending a corresponding message.
//
// This function returns false only in the case that a disconnect request
// has been sent. Note that all other requests will be serviced before
// the connection is closed.
bool
Protocol::service(Reactor& r) {
  bool result = true;
  Request_queue& reqs = switch_->requests();
  while (not reqs.empty()) {
    Request req = reqs.front();
    reqs.pop();
    result &= service(r, req);
  }
  return result;
}

/// Dispatch an appropriate response to the request. This function
/// only returns false if a disconnection event is serviced.
bool
Protocol::service(Reactor& r, const Request& req) {
  switch (req.type) {
  case Request::DISCONNECT: return on_disconnect(r, req);
  case Request::TERMINATE: return on_terminate(r, req);
  default: return true;
  }
}

/// Disconnect the switch. This returns false, causing the reactor to
/// close the connection.
bool
Protocol::on_disconnect(Reactor& r, const Request&) { 
  return false; 
}

/// Terminate the application on this switch. Note that the application
/// will continue to be loaded, just not running on the switch.
bool
Protocol::on_terminate(Reactor& r, const Request& req) {
  switch_->unbind(req.app);
  return true;
}

} // namespace ofp
} // namespace freeflow