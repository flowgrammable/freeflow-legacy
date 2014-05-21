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

#include <iostream>

#include "template.hpp"

using namespace freeflow;

namespace nocontrol {
  
Application* 
Factory::make() {
  std::cout << "New instance of application 'Template' has been created\n";
  return new Template;
}

void 
Factory::destroy(Application* n) {
  std::cout << "An instance of application 'Template' has been deleted\n";
  delete n;
}

/// The application factory.
static Factory Template_factory;


void
Template::load(Controller& ctrl) { 
  std::cout << "Application 'Template' loaded to the controller\n";
}

void
Template::unload(Controller& ctrl) { 
  std::cout << "Application 'Template' unloaded from the controller\n";
}



} // namespace nocontrol

extern "C" Application_factory*
factory() { return &nocontrol::Template_factory; }
