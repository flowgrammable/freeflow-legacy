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
#define NOCONTROL_NOWFLOW_HPP

#include <freeflow/sdn/application.hpp>
#include <freeflow/sdn/controller.hpp>
#include <freeflow/sdn/switch.hpp>

#include <nocontrol/config.hpp>

namespace nocontrol {
  
class Factory;
class Noflow;

/// The noflow 
class Noflow : public ff::Application {
  enum Stop { ON_BIND, ON_VERSION, ON_FEATURE };
public:
  void load(ff::Controller&);
  void bind(ff::Switch&);
  void version_known(ff::Switch&);
  void features_known(ff::Switch&);
  void test();

private:
  Stop stop_ = ON_BIND; // The stopping point.
};


/// Create instances of the noflow application.
class Factory : public ff::Application_factory {
  ff::Application* make();
  void destroy(ff::Application*);
};
  
} // namespace nocontrol

#endif
