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

#ifndef FREEFLOW_APPLICATION_HPP
#define FREEFLOW_APPLICATION_HPP

namespace freeflow {

class Controller;
class Switch;
class Packet;
class Flow;
class Port;
class Table;
class Role;

/// The base class of all native Freeflow applications. This defines the
/// abstract events sent to derived classes.
class Application {
public:
  virtual ~Application() { }

  // Application events
  virtual bool load(Controller&);
  virtual bool unload(Controller&);

  // Transport events
  virtual bool bind(Switch&);
  virtual bool unbind(Switch&);

  // Discovery events
  virtual bool version_known(Switch&);
  virtual bool features_known(Switch&);

  // Startup events
  virtual bool start(Switch&);
  virtual bool stop(Switch&);

  // Datapath events
  virtual bool packet_in(Switch&, const Packet&);
  virtual bool flow_removed(Switch&, const Flow&);
  virtual bool port_status(Switch&, const Port&);
  virtual bool table_status(Switch&, const Table&);
  virtual bool role_status(Switch&, const Role&);
};

} // namespace freeflow

#include <freeflow/nbi/application.ipp>

#endif

