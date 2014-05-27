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

#include <freeflow/sys/socket.hpp>
#include <freeflow/sdn/application.hpp>

namespace freeflow {

struct Switch;

/// The Controller class is...
///
/// \todo Force the controller to manage plugins? Or make that the
/// responsibility of some other component.
class Controller {
  using App_set = std::unordered_set<Application*>;
  using Switch_set = std::unordered_set<Switch*>;

public:
  // void load(Application_library&);
  // void unload(Application_library&);

  // Switch management
  Switch& connect(Socket&);
  void disconnect(Switch&);

private:
  App_set    apps_;     // The hosted applications
  Switch_set switches_; // Connected switches
};

} // namespace freeflow

#include <freeflow/sdn/controller.ipp>

#endif

