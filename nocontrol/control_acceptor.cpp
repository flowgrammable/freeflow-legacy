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

// Don't create a new handler for the connection. Instead,
// just read off the data and do something with it.
//
// FIXME: Lots and lots of work is needed here. This should probably
// spin up a management connection, even though this is really a
// stateless protool. That protocol would simply override the on_open
// function so that it performed all these tasks and the removed itself
// from the reactor. Seems legit.
ff::Event_handler* 
Control_factory::operator()(Reactor& r, Socket&& s) {
  Socket sock = std::move(s);
  
  // Read 4K worth of data. If we don't read anything allow the
  // socket to close.
  //
  // FIXME: This should be streamed or maybe something else.
  constexpr std::size_t N = 4096;
  Buffer buf(N);
  System_result res = read(sock, buf);

  // Check for failures.
  if (res.failed()) {
    std::cerr << "failed to read from client\n";
    return nullptr; 
  }
  
  // If we didn't ready any data, the socket is closed.
  // Note that we're guaranteed completion.
  if (res.value() != 0) {
    // Decode the JSON value.
    // FIXME: This is a potentially slow operation that could
    // impact server performance.
    json::Value v = json::parse(buf);

    // FIXME: Actually do something with the parsed value
    // besides printing it!.
    std::cout << v << '\n';
  }

  return nullptr;
}

} // namesapce nocontrol
