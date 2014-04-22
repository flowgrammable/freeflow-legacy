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

/// Return the controller associated with the swithc.
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

/// Sets the protocol and version and experiment flags.
inline void
Switch::set_protocol(Uint8 v, Uint8 e) {
  proto_vsn = v;
  proto_exp = e;
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

} // namespace freeflow
