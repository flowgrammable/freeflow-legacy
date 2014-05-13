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
#include <freeflow/sys/json.hpp>

#include "control_acceptor.hpp"
#include "connection.hpp"

using namespace ff;

namespace nocontrol {

// When data is available for reading, accept the connection
// and spawn a new handler.
bool
Control_acceptor::on_read(Reactor& r) {
  try {
    // Accept the connection.
    Socket s = rc().accept();

    // Read 4K worth of data. If we don't read anything allow the
    // socket to close.
    //
    // FIXME: This should be streamed or maybe something else.
    constexpr std::size_t N = 4096;
    Buffer buf(N);
    System_result r = read(s, buf);
    if (r.failed()) {
      std::cerr << "failed to read from client\n";
      return true; 
    }
    
    // If we didn't ready any data, the socket is closed.
    // Note that we're guaranteed completion.
    if (r.value() == 0)
      return true;

    // Decode the JSON value.
    // FIXME: This is a potentially slow operation that could
    // impact server performance.
    json::Value v = json::parse(buf);

    // FIXME: Actually do something with the parsed value
    // besides printing it!.
    std::cout << v << '\n';

  } catch(...) {
    // FIXME: Improve this.
    std::cerr << "error accepting connection\n";
  }

  // This never terminates...
  return true;
}

} // namesapce nocontrol
