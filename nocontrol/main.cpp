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

#include "connection.hpp"

using namespace std;
using namespace freeflow;


// The handler registry maintains the set of handlers registered
// for the reactor loop.
//
// FIXME: This could also house the wait set: the fd set that describes
// what the handler is waiting on.
struct Handler_registry : std::map<int, Handler*> {
  
  // Register the handler.
  void add(Handler* h) { insert({h->fd(), h}); }

  // Remove the hander.
  void remove(Handler* h) { erase(h->fd()); }
};

// Gobal handler registry.
Handler_registry handlers;




// This handler is responsible for watching for the end of
// file from an open file.
struct Terminator : Resource_handler<Resource> {
  Terminator(int fd)
    : Resource_handler<Resource>(fd) { }

  Terminator(Resource&& f)
    : Resource_handler<Resource>(std::move(f)) { }

  // Red
  Result on_read() {
    char c[1024];
    if (read(rc(), &c, 1024) <= 0)
      return EXIT;
    return CONTINUE;
  }
};

// The acceptor is responsible for accepting connections when
// they are available.
struct Acceptor : Resource_handler<Socket> {
  Acceptor(const Address& a)
    : Resource_handler<Socket>(a.family(), Socket::TCP) 
  {
    rc().bind(a); 
    rc().listen();
  }

  // When data is available for reading, accept the connection
  // and spawn a new handler.
  Result on_read() {
    // FIXME: The error handling stuff is not good.
    try {
      Connection* c = new Connection(rc().accept());
      handlers.add(c);
    } catch(System_error&) {
      perror("error");
      return EXIT;
    } catch(std::runtime_error& err) {
      std::cout << err.what() << '\n';
      return EXIT;
    }
    return CONTINUE;
  }
};


void
register_handler(Resource_set& rs, Handler& h) {
  rs.insert(h.fd());
}

void
register_handlers(Resource_set& rs, const Handler_registry& hs) {
  for (const auto& x : hs)
    register_handler(rs, *x.second);
}


int 
main(int argc, char* argv[]) {

  // Configure the switch address.
  Address addr(Ipv4_addr::any, 9001);

  Terminator term(0);
  Acceptor acc(addr);

  // Register default service handlers.
  // FIXME: nlogn lookup for file descriptors is slow.
  handlers.add(&term);
  handlers.add(&acc);

  bool done = false;
  while (not done) {
    int maxfd = (*--handlers.end()).second->fd() + 1;

    // Build the wait set.
    //
    // TODO: We should only have to do this if the handler set has
    // changed or if a handler is registered for a new event.
    Resource_set wait;
    register_handlers(wait, handlers);

    // Create the dispatch set. The dispatch set is modified
    // by select to indicate which files have events.
    Resource_set disp = wait;

    // Select events.
    Selector s(maxfd, &disp);    
    s();

    // Handle events, saving any handlers that self-close.
    Resource_set close;
    for (const auto& x : handlers) {
      Handler* h = x.second;
      if (disp.test(h->fd())) {
        Result r = h->on_read();
        if (r == STOP)
          close.insert(h->fd());
        else if (r == EXIT)
          done = true;
      }
    }


    // Close any handlers that need to be destroyed and
    // cull them from the registry.
    for (auto i = handlers.begin(); i != handlers.end();) {
      Handler* h = i->second;
      if (close.test(h->fd())) {
        h->close();
        i = handlers.erase(i);
      } else {
        ++i;
      }
    }

  } // while(not done)

  return 0;
}
