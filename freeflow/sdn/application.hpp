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

#include <freeflow/sys/library.hpp>

namespace freeflow {

class Controller;
class Switch;
class Packet;
class Flow;
class Port;
class Flow_table;
class Role;
class Application;

/// The Application_factory is an abstract base class whose derived 
/// classes are responsible for creating applications. Objects of this 
/// class are returned from the loading of application libraries.
class Application_factory {
public:
  virtual Application* create(Controller&) = 0;
  virtual void destroy(Application*) = 0;
};

/// The type of the factory function pointer
using Application_factory_fn = Application_factory*(*)();


/// The Application_library class represents a dynamically loaded
/// library that implements (one or more?) freeflow applications.
/// The factory is used to instantiate those applications whenever
/// they are launched.
class Application_library : Library {
public:
  Application_library(const Path&);
  
  // Accessors
  Application_factory* factory() const;
  
  // Application management
  Application* create(Controller&) const;
  void destroy(Application*) const;
  
private:
  Application_factory_fn entry_;   // Resolved etnry point
  Application_factory*   factory_; // The factory object
};


/// The base class of all native Freeflow applications. This defines the
/// abstract events sent to derived classes.
class Application {
public:
  Application(Controller&);
  virtual ~Application();

  Controller& controller();

  // Transport events
  virtual void bind(Switch&);
  virtual void unbind(Switch&);

  // Discovery events
  virtual void version_known(Switch&);
  virtual void features_known(Switch&);

  // Startup events
  virtual void start();
  virtual void stop();

  // Datapath events
  virtual void packet_in(Switch&, const Packet&);
  virtual void flow_removed(Switch&, const Flow&);
  virtual void port_status(Switch&, const Port&);
  virtual void table_status(Switch&, const Flow_table&);
  virtual void role_status(Switch&, const Role&);

private:
  Controller& ctrl_;
};


} // namespace freeflow

#include <freeflow/sdn/application.ipp>

#endif

