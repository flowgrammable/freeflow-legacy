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

#include <unordered_map>

namespace freeflow {

struct Switch;

/// The Controller class represents...
class Controller {
  using Switch_map = std::unordered_map<std::size_t, Switch*>;

  Switch& connect();
  void disconnect(Switch&);

private:
  Switch_map switches_;
};

} // namespace freeflow

#include "controller.ipp"

#endif

