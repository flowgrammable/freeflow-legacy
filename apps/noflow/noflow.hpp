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

#ifndef NOCONTROL_NOFLOW_HPP
#define NOCONTROL_NOFLOW_HPP

#include <freeflow/sdn/application.hpp>
#include <freeflow/sdn/controller.hpp>
#include <freeflow/sdn/switch.hpp>

namespace ff = freeflow;

class Factory;
class Noflow;

/// The Noflow application is a diagnostic tool used to echo events
/// sent from a swith.
class Noflow : public ff::Application {
public:
  Noflow(ff::Controller&);
};


/// Create instances of the Noflow application.
class Factory : public ff::Application_factory {
  ff::Application* create(ff::Controller&);
  void destroy(ff::Application*);
};

#endif
