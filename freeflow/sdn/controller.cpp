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

#include "controller.hpp"
#include "switch.hpp"

namespace freeflow {

/// Register the switch and invoke its bind event.
Switch&
Controller::connect(Socket& sock) {
  auto i = switches_.emplace(new Switch(*this, sock));
  Switch& s = **i.first;

  // TODO: Find the set applications to bind to the connected switch.
  // We need to consult some configuration table to determine this.
  // For now, I'm assuming that all applications bind to every switch.
  s.bind(app_);

  return **i.first;
}

/// Remove the switch from the set.
void
Controller::disconnect(Switch& s) {
  s.unbind();
  switches_.erase(&s);
  delete &s;
}

} // namespace freeflow
