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

#include "acceptor.hpp"
#include "connection.hpp"

using namespace std;
using namespace freeflow;
using namespace nocontrol;


// This handler is responsible for watching for the end of
// file from an open file.
struct Terminator : Resource_handler<Resource> {
  Terminator(int fd)
    : Resource_handler<Resource>(fd) { }

  Terminator(Resource&& f)
    : Resource_handler<Resource>(std::move(f)) { }

  // If there is no more data to read, indicate that we want
  // to terminate the reactor loop.
  Result on_read() {
    char c[1024];
    if (read(rc(), &c, 1024) <= 0) {
      std::cout << "shutting down\n";
      return EXIT;
    }
    return CONTINUE;
  }
};


void
register_handlers(Resource_set& rs, const Handler_registry& r) {
  for (Handler* h : r)
    if (h) rs.insert(h->fd());
}

bool
notify_read(Handler* h, const Resource_set& read, Resource_set& close) {
  if (read.test(h->fd())) {
    Result r = h->on_read();
    if (r == STOP)
      close.insert(h->fd());
    else if (r == EXIT)
      return true;
  }
  return false;
}

bool
notify_handlers(Handler_registry& r, const Resource_set& read, Resource_set& close) {
  bool exit = false;
  for (Handler* h : r)
    if (h) exit |= notify_read(h, read, close);
  return exit;
}

void
close_handlers(Handler_registry& r, const Resource_set& close) {
   for (Handler* h : r) {
    if (h and close.test(h->fd()))
      r.remove(h);
  } 
}

int 
main(int argc, char* argv[]) {

  // Configure the switch address.
  Address addr(Ipv4_addr::any, 9001);
  Acceptor acc(addr);

  // Listen for ^D on stdin so we can shutdown easily.
  Terminator term(0);

  // Register default service handlers.
  Handler_registry& handlers = Handler_registry::instance();
  handlers.add(&term);
  handlers.add(&acc);

  bool done = false;
  while (not done) {
    // Build the wait set.
    //
    // TODO: We should only have to do this if the handler set has
    // changed or if a handler is registered for a new event.
    Resource_set wait;
    register_handlers(wait, handlers);

    // Create the dispatch set. The dispatch set is modified
    // by select to indicate which files have events.
    // FIXME: Add resource sets for write and error.
    Resource_set disp = wait;
    Selector s(handlers.max() + 1, &disp);    
    s();

    // Close any handlers that need to removed from the
    // registry and closed.
    Resource_set close;
    done = notify_handlers(handlers, disp, close);
    
    // Before exiting, close any outstanding handlers
    close_handlers(handlers, close);

  } // while(not done)


  // Close all handlers.
  // FIXME: This should probably run in reverse, but maybe not...
  for (Handler* h : handlers) {
    if (h)
      handlers.remove(h);
  }

  return 0;
}
