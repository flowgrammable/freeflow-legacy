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

#include "switch_acceptor.hpp"
#include "control_acceptor.hpp"
#include "connection.hpp"
#include "noflow.hpp"
#include "bridge.hpp"

using namespace std;
using namespace freeflow;
using namespace nocontrol;

// This handler is responsible for watching for the end of
// file from an open file. 
//
// This is primarily intended for debugging purposes. Really, we should
// be trapping signals and shutting down that way instead.
struct Terminator : Resource_handler {
  Terminator(Reactor& r, int fd)
    : Resource_handler(r, READ_EVENTS, fd) { }

  // If there is no more data to read, indicate that we want
  // to terminate the reactor loop.
  bool on_read() {
    char c[1024];
    if (read(rc(), &c, 1024) <= 0) {
      std::cout << "shutting down\n";
      reactor().stop();
      return false;
    }
    return true;
  }
};


// Stores configuration information for the controller.
// FIXME: This needs to move into nbi.
struct Controller_config {
  Address ctrl_addr {Ipv4_addr::any, 9000};
  Address mgmt_addr {Ipv4_addr::any, 9001};
};

int 
main(int argc, char* argv[]) {

  Reactor r;

  // FIXME: Write a command-line argument parser and make sure
  // that this won't crash (it currently will).

  // Create and configure the controller for NBI applications.
  Controller ctrl;
  Controller_config conf;

  // Load default applications.
  ctrl.load<Noflow>();

  // Configure the conntroller adress.
  Switch_acceptor ctrl_acc(r, ctrl, conf.ctrl_addr);
  Control_acceptor mgmt_acc(r, ctrl, conf.mgmt_addr);
  Terminator term(r, 0);
  r.add_handler(&term);
  r.add_handler(&ctrl_acc);
  r.add_handler(&mgmt_acc);

  // Run the reactor loop.
  r.run();

  // FIXME: This should be part of the controller's destructor.
  ctrl.unload<Noflow>();

  return 0;
}
