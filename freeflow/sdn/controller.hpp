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

#ifndef FREEFLOW_CONTROLLER_HPP
#define FREEFLOW_CONTROLLER_HPP

#include <list>
#include <unordered_map>
#include <unordered_set>

#include <freeflow/sys/acceptor.hpp>
#include <freeflow/sys/connector.hpp>
#include <freeflow/sys/reactor.hpp>

#include <freeflow/sdn/application.hpp>

namespace freeflow {

struct Switch;
struct Process;
struct Controller;

/// A positoin in a process list.
using Process_iterator = std::list<Process>::iterator;


/// A process represents an instance of a running application. It
/// tracks book-keeping information related to the process' running.
///
/// \todo This should probably be in its own module.
///
/// \todo A process should have a process identifier. Probably some
/// integer, but also possibly a name?
struct Process {
  Process(Application*, Application_library*);

  Application*         app; // The actual application
  Application_library* lib; // The source library
  
  Process_iterator     pos; // Internal position in application list
};



/// The Controller class is a reactor that provides services for running
/// a service that queries, configures, or controls some component of the 
/// SDN data model.
///
/// Note that it is possible to write primary Nocontrol services like
/// the OpenFlow controller as applications. This is made possible because
/// the registration of listeners can occur during application startup.
/// The same is also true of applicaitons that must connect to remote
/// services.
///
/// \todo Maybe this should be called agent since it could conceivably
/// be used as the basis for building either controller or switch agent.
///
/// \todo This class leaks memory.
///
/// \todo Refactor the library loading features into a separate class.
/// The controller should simply inherit those capabilities.
///
/// \todo Refactor process management into a separate class.
class Controller : public Reactor {
  using Library_map = std::unordered_map<std::string, Application_library*>;
  using Process_list = std::list<Process>;
  using Switch_set = std::unordered_set<Switch*>;
public:
  template<typename Handler>
    struct Handler_factory;

  template<typename Handler, typename Factory = Handler_factory<Handler>>
    struct Acceptor;

  template<typename Handler, typename Factory = Handler_factory<Handler>>
    struct Connector;


  // Listener and connector management
  template<typename T>
    void add_acceptor(T*, const Address&, Socket::Transport);

  template<typename T>
    void add_connector(T*, const Address&, Socket::Transport);

  // Application loading
  Application_library* load(const std::string&);
  void unload(const std::string&);
  bool is_loaded(const std::string&);

  // Process managment
  Process* start(const std::string&);
  void stop(Process*);

  // Switch management
  Switch& connect(Socket&);
  void disconnect(Switch&);

private:
  Library_map  libs_;     // The set of libraries
  Process_list procs_;    // The hosted applications
  Switch_set   switches_; // Connected switches
};

/// The Handler_factory is responsible for the allocation of event
/// handlers for for a given controller.
template<typename Handler>
  struct Controller::Handler_factory {
    Handler_factory(Controller&);

    Handler* operator()(Reactor&, Socket&&) const;

  private:
    Controller& ctrl_;
  };

/// An acceptor specifically bound to a controller.
template<typename Handler, typename Factory>
  struct Controller::Acceptor : freeflow::Acceptor<Handler, Factory> {
    Acceptor(Controller& ctrl);
  };


/// An acceptor specifially bound to a controller.
template<typename Handler, typename Factory>
  struct Controller::Connector : freeflow::Connector<Handler, Factory> {
    Connector(Controller& ctrl);
  };

} // namespace freeflow

#include <freeflow/sdn/controller.ipp>

#endif

