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

#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/ofp.hpp>

#include "openflow.hpp"

using namespace freeflow;

namespace nocontrol {

/// Create a state machine and initiate version negotiation. 

bool
Ofp_handler::on_open() {
  std::cout << "* OFP switch connected\n";
  return true;
}

bool 
Ofp_handler::on_close() {
  return true; 
}

bool
Ofp_handler::on_read() {
  return true;
}

/// When a timeout occurs, notify the protocol of the expired timer.
bool
Ofp_handler::on_time(int t) {
  return true;
}

} // namespace nocontrol

