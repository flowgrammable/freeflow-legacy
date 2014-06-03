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

#include "machine.hpp"

namespace freeflow {
namespace ofp {
namespace v1_0 {

bool
Machine::on_init() {
  state_ = INIT;
  send_hello();
  return true;
}

// We've transferred into a closing state.
bool
Machine::on_close() {
  return false;
}

bool
Machine::on_message() {
  // Peek at the header.
  ofp::Header h;
  ch_.recv.peek_msg(h);

  // FIXME: Validate the version.

  // FIXME: We sohuld only have feature messages here.
  // if (state_ == INIT) {
  //   if (h.type == ofp::v1_0::HELLO)
  //     return on_version();
  //   else
  //     return on_close();
  // } else if (state_ == VERSION) {

  // } else if (state_ == RUN) {

  // }
  return true;
}

} // namespace v1_0
} // namespace ofp
} // namespace nocontrol
