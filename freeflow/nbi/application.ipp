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

/// The load event is sent when the controller instantiates the application.
inline bool
Application::load(Controller&) { return true; }

/// The unload event is sent when the controller tears down the application.
inline bool
Application::unload(Controller&) { return true; }

/// The bind event is sent whenever the application is bound to a switch.
/// This happens immediately after the switch connects to the controller.
inline bool
Application::bind(Switch&) { return true; }

/// The unbind event is sent just before the controller disconnects from
/// the switch.
inline bool
Application::unbind(Switch&) { return true; }

/// The version known event is sent after the protocol and version
/// have been negotiated and before the the start event is sent.
inline bool
Application::version_known(Switch&) { return true; }

/// The features known event is sent after feature discovery has
/// completed and before the start event is sent.
inline bool
Application::features_known(Switch&) { return true; }

/// The start event is sent when an application is first instantiated
/// on the given switch. The start event intended to be point at which
/// an application establishes all necessary resources to function.
/// Examples include the announcement of program requirements, the
/// allocation of tables, and the allocation of groups.
inline bool
Application::start(Switch&) { return true; }

/// The stop event is sent when an application is uninstalled, either
/// because the connection was closed or because an operator requested
/// shutdown. Any resources acquired during startup must be released
/// during shutdown.
inline bool
Application::stop(Switch&) { return true; }

/// The packet-in event is sent whenever a packet is transferred to
/// the controller due to a table miss or a flow configuration.
inline bool
Application::packet_in(Switch&, const Packet&) { return true; }

/// The flow-removed event is sent whenever a flow is removed from an
/// acquired flow table.
inline bool
Application::flow_removed(Switch&, const Flow&) { return true; }

/// The port-status event is sent when a physical port is added, removed,
/// or modified by the path.
///
/// In the case where the port is removed, the internal representation
/// is released only after all applications have been notified.
inline bool
Application::port_status(Switch&, const Port&) { return true; }

/// \todo Document me.
inline bool
Application::table_status(Switch&, const Table&) { return true; }

/// \todo Document me.
inline bool
Application::role_status(Switch&, const Role&) { return true; }

} // namespace freeflow

