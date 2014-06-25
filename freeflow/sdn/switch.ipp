// Copyright (c) 2013-2014 Flowgrammable, LLC.
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

/// Initialize the switch object for the given controller.
inline
Switch::Switch(Controller& c, Socket& s)
  : ctrl_(c), sock_(s) { }

/// Return the controller associated with the switch.
inline Controller& 
Switch::controller() { return ctrl_; }

/// Returns the negotiated protocol version.
///
/// \todo Find a more abstract scheme for managing the protocol. Also,
/// handle experimental variants of the protocol correctly.
inline Uint8
Switch::protocol_version() const { return proto_vsn; }

/// Returns true if the version is designated as experimental.
inline Uint8
Switch::protocol_experiment() const { return proto_exp; }

/// Sets the protocol and version and experiment flags. Hosted applications
/// are notified of the version discovery. Returns true if the protocol
/// is configured correctly.
inline void
Switch::set_protocol(Uint8 v, Uint8 e) {
  proto_vsn = v;
  proto_exp = e;
  app_->version_known(*this);
}

/// Extract the nth bit from the number passed in.
/// \todo where to move this?
inline bool
get_bit(int number, int n) { return (number & (1 << n)) >> n; }

inline void 
Switch::features_config(ofp::Header h, ofp::v1_0::Feature_reply r) {
  // Configure datapath members
  dp_.datapath_id   = r.datapath_id;
  dp_.ip_reassembly = get_bit(r.capabilities, 6);

  // Configure ports
  for (const ofp::v1_0::Port& port : r.ports) {
    Port p;
    p.port_number = port.port_id;
    p.hw_addr = port.hw_addr;
    p.name = port.name.str();
    // p.speed 
    // p.mode
    // p.medium
    // p.auto_neg
    // p.pause
    // p.pause_asym

  }
}

/// Indicate that the switch is ready to begin operation. The features-known
/// event is sent to loaded applications. Those applications may or may not 
/// be started, depending on configuration.
inline void
Switch::configured() {
  app_->features_known(*this);

  // TODO: Auto-start applications? Should the controller have a special
  // auto-start application that listens and requests application startup?
}

/// Bind given application to the switch.
inline void
Switch::bind(Application* app) {
  app_ = app;
  app_->bind(*this);
}

/// Unbind the given application from the switch.
inline void
Switch::unbind(Application* app) {
  assert(app_ == app);
  app_->unbind(*this);
  app_ = nullptr;
}

/// Unbind all applications from the switch.
///
/// \todo Actually unbind multiple applications.
inline void
Switch::unbind() { unbind(app_); }

/// Request that the switch be disconnected.
///
/// \todo Verify that the application has permission to close the
/// connection.
inline void
Switch::disconnect() {
  // FIXME: Invent a framework that maintains the current application
  // when dispatching events. That way, the requesting application does't
  // have to keep passing itself back to the controller when sending
  // events.
  reqs_.emplace(app_, Disconnect_request{});
}

/// Request that the application be terminated on this switch.
inline void
Switch::terminate() {
  reqs_.emplace(app_, Terminate_request{});
}

/// Returns a reference to the request queue, allowing a protocol
/// implementation to service any application requsts.
inline Request_queue&
Switch::requests() { return reqs_; }

} // namespace freeflow
