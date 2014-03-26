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

#include "connection.hpp"

namespace nocontrol {

// Create a version negotiation state machine so we know what version
// we should accept.
void
Connection::open() {

}

// Shutdown the state machine and delete the handler.
void 
Connection::close() { 
  delete this; 
}

Result 
Connection::on_read() {
  char buf[1024];
  std::size_t n = rc().read(buf, 1024);
  if (n == 0)
    return STOP;

  buf[n] = 0;
  std::cout << "read: " << buf << '\n';

  return CONTINUE;
}

Result
Connection::on_write() { return CONTINUE; }

} // namespace nocontrol
