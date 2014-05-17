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

#include <freeflow/sys/error.hpp>

#include "switch_acceptor.hpp"
#include "connection.hpp"

using namespace ff;

namespace nocontrol {

// When data is available for reading, accept the connection
// and spawn a new handler.
bool
Switch_acceptor::on_read() {
  // FIXME: The error handling stuff is not good. Note that even in case 
  // of errors, the acceptor never self-terminates.
  Socket s;
  try {
    s = rc().accept();
  } catch(System_error&) {
    perror("error");
    reactor().stop();
    return true;
  } catch(std::runtime_error& err) {
    std::cout << err.what() << '\n';
    reactor().stop();
    return true;
  }

  // Create the new service handler.
  reactor().new_handler<Connection>(ctrl_, std::move(s));
  return true;
}

} // namesapce nocontrol
