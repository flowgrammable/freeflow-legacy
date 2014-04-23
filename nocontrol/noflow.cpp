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

#include "noflow.hpp"

using namespace freeflow;

namespace nocontrol {

bool
Noflow::load(Controller&) {
  stop_ = BIND;
  std::cout << "loading noflow\n";
  return true;
}

bool
Noflow::unload(Controller&) {
  std::cout << "unloading noflow\n";
  return true;
}

bool
Noflow::bind(Switch& s) {
  std::cout << "bind to switch\n";
  s.disconnect();
  return true;
}

bool
Noflow::unbind(Switch& s) {
  std::cout << "unbind from switch\n";
  return true;
}

bool
Noflow::version_known(Switch& s) {
  std::cout << "version negotiated\n";
  return stop_ != VERSION;
}

bool
Noflow::features_known(Switch& s) {
  std::cout << "features discovered\n";
  return false;
}

} // namespace nocontrol
