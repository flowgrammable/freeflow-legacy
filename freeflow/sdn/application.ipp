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


inline
Application_library::Application_library(const Path& p) : Library(p)
{
  factory_ = this->function<Application_factory_fn>("factory")();
}

inline Application_factory*
Application_library::factory() const {
  return factory_;
}

/// The load event is sent when the controller instantiates the application.
inline void
Application::load(Controller&) { }

/// The unload event is sent when the controller tears down the application.
inline void
Application::unload(Controller&) { }

/// The bind event is sent whenever the application is bound to a switch.
/// This happens immediately after the switch connects to the controller.
inline void
Application::bind(Switch&) { }

/// The unbind event is sent just before the controller disconnects from
/// the switch.
///
/// No switch requests may be when processing this event.
inline void
Application::unbind(Switch&) { }

/// The version known event is sent after the protocol and version
/// have been negotiated and before the the start event is sent.
inline void
Application::version_known(Switch&) { }

/// The features known event is sent after feature discovery has
/// completed and before the start event is sent.
inline void
Application::features_known(Switch&) { }

/// The start event is sent when an application is first instantiated
/// on the given switch. The start event intended to be point at which
/// an application establishes all necessary resources to function.
/// Examples include the announcement of program requirements, the
/// allocation of tables, and the allocation of groups.
inline void
Application::start(Switch&) { }

/// The stop event is sent when an application is uninstalled, either
/// because the connection was closed or because an operator requested
/// shutdown. Any resources acquired during startup must be released
/// during shutdown.
inline void
Application::stop(Switch&) { }

/// The packet-in event is sent whenever a packet is transferred to
/// the controller due to a table miss or a flow configuration.
inline void
Application::packet_in(Switch&, const Packet&) { }

/// The flow-removed event is sent whenever a flow is removed from an
/// acquired flow table.
inline void
Application::flow_removed(Switch&, const Flow&) { }

/// The port-status event is sent when a physical port is added, removed,
/// or modified by the path.
///
/// In the case where the port is removed, the internal representation
/// is released only after all applications have been notified.
inline void
Application::port_status(Switch&, const Port&) { }

/// \todo Document me.
inline void
Application::table_status(Switch&, const Table&) { }

/// \todo Document me.
inline void
Application::role_status(Switch&, const Role&) { }

} // namespace freeflow

