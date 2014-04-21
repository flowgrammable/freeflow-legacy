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
#include <map>
#include <typeinfo>

#include <freeflow/sys/selector.hpp>
#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/file.hpp>
#include <freeflow/sys/reactor.hpp>
#include <freeflow/nbi/controller.hpp>

#include "acceptor.hpp"
#include "connection.hpp"
#include "bridge.hpp"

using namespace std;
using namespace freeflow;
using namespace nocontrol;

// This handler is responsible for watching for the end of
// file from an open file. 
//
// This is primarily intended for debugging purposes. Really, we should
// be trapping signals and shutting down that way instead.
struct Terminator : Resource_handler<Resource> {
  Terminator(int fd)
    : Resource_handler<Resource>(fd) { }

  Terminator(Resource&& f)
    : Resource_handler<Resource>(std::move(f)) { }

  // If there is no more data to read, indicate that we want
  // to terminate the reactor loop.
  bool on_read(Reactor& r) {
    char c[1024];
    if (read(rc(), &c, 1024) <= 0) {
      std::cout << "shutting down\n";
      r.stop();
      return false;
    }
    return true;
  }
};


int 
main(int argc, char* argv[]) {

  // Configure the switch address.
  Address addr(Ipv4_addr::any, 9001);
  Acceptor acc(addr);

  // Listen for ^D on stdin so we can shutdown easily.
  Terminator term(0);

  // Create the controller for NBI applications.
  Controller ctrl;

  // Create the application running on the controller.
  //
  // FIXME: How do we know what switches the application should
  // be deployed on? Certainly not all of them.
  Bridge app;

  // Run the reactor loop.
  //
  // FIXME: Consider moving all of the reactor facilities into
  // the NBI library.
  Reactor r;
  r.add_handler(&term);
  r.add_handler(&acc);
  r.run();

  return 0;
}
