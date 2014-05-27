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

#include <freeflow/sys/json.hpp>

#include "nocontrol.hpp"

namespace nocontrol {

using namespace freeflow;

bool
Nocontrol_handler::on_read() {
  System_result res = rc().read(buf_);

  // Check for failures.
  // FIXME: Improve error handling.
  if (res.failed()) {
    std::cerr << "failed to read from client\n";
    return false; 
  }

  // If we read 0, bytes, the connection is closed.
  if (res.value() == 0)
    return false;

  // Decode and dispatch the command.
  // FIXME: Actually do something here! Also improve error handling.
  try {
    json::Value v = json::parse(buf_);
    std::cout << v << '\n';
  } catch (...) {
    std::cerr << "error: ill-formed command\n";
    return false;
  }

  return true;
}

} // namesapce nocontrol
