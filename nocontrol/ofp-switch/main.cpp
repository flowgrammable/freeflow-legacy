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
#include <dlfcn.h>

#include <freeflow/sys/selector.hpp>
#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/file.hpp>
#include <freeflow/sys/reactor.hpp>
#include <freeflow/sdn/controller.hpp>
#include <apps/noflow/noflow.hpp>

#include "prelude.hpp"
#include "nocontrol.hpp"
#include "openflow.hpp"

using namespace std;
using namespace freeflow;
using namespace nocontrol;

// This signal handler is responsible for implementing graceful
// shutdown on termination signals.
struct Signal_handler : Resource_handler {
  Signal_handler(Reactor& r)
    : Resource_handler(r, SIGNAL_EVENTS, 0) { }

  bool on_signal(int s) {
    std::cout << "* shutting down\n";
    switch (s) {
    case SIGINT:
    case SIGTERM:
    case SIGQUIT:
      reactor().stop();
      break;
    default:
      break;
    }
    return true;
  }
};


int 
main(int argc, char* argv[]) {
  Controller c;

  // FIXME: All of this information needs to come from
  // the configuration (files, command line, etc).
  constexpr ff::Socket::Transport tcp = ff::Socket::TCP;
  Address ofp_addr {Ipv4_addr::any, 9000};
  // Address ncp_addr {Ipv4_addr::any, 9001};

  // Ncp_acceptor ncp(c);
  // c.add_acceptor(&ncp, ncp_addr, tcp);
  
  Ofp_connector ofp(c);
  c.add_connector(&ofp, ofp_addr, tcp);

  // Add the signal handler
  Signal_handler sh(c);
  c.add_handler(&sh);

  c.run();

  return 0;
}
