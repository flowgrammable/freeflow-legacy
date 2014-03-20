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

using namespace std;
using namespace freeflow;

// Result codes for events.
// TODO: Find a better name.
enum Result {
  CONTINUE = 0,
  STOP = -1,
  EXIT = -2
};

class Handler {
public:
  Handler() = delete;

  // Non-copyable
  Handler(const Handler&) = delete;
  Handler& operator=(const Handler&) = delete;

  // Move semantics
  Handler(Handler&& x)
    : rc_(std::move(x.rc_)) { }

  Handler& operator=(Handler&& x) {
    rc_ = std::move(x.rc_);
    return *this;
  }

  Handler(Resource&& r)
    : rc_(std::move(r)) { }

  // Returns the underlying resource.
  Resource& resource() { return rc_; }

  // Returns the underlying file descriptor.
  int fd() const { return rc_.fd(); }

  // Event operators
  virtual void open() { }
  virtual void close() { }

  virtual Result on_read() { return CONTINUE; }
  virtual Result on_write() { return CONTINUE; }
  virtual Result on_error() { return CONTINUE; }
  virtual Result on_time() { return CONTINUE; }

private:
  Resource rc_;
  bool done_;
};

using Handler_map = std::map<int, Handler*>;


// Represents a connected switch.
//
// TODO: In full generality, this should be a service that sits on
// an underlying resource. However, we don't really need full
// generality just yet.
struct Connection : Handler {
  using Handler::Handler;

  // Move semantics
  Connection(Connection&& x)
    : Handler(std::move(x)) { }

  Socket& socket() { return static_cast<Socket&>(resource()); }

  // Data is available for reading.
  Result on_read() {
    // Try reading from the buffer. 
    char buf[1024];
    std::size_t n = socket().read(buf, 1024);
    if (n == 0)
      return STOP;
    
    buf[n] = 0;
    return CONTINUE;
  }

  // Data is available for writing.
  Result on_write() { return CONTINUE; }
};


// This handler is responsible for watching for the end of
// file from an open file.
struct Terminator : Handler {
  Terminator(int fd)
    : Handler(Resource(fd)) { }

  Terminator(File&& f)
    : Handler(std::move(f)) { }

  File& file() { return static_cast<File&>(resource()); }

  // Red
  Result on_read() {
    char c[1024];
    if (read(file(), &c, 1024) <= 0)
      return EXIT;
    return CONTINUE;
  }
};


// The acceptor is responsible for accepting connections when
// they are available.
struct Acceptor : Handler {
  Acceptor(const Address& a)
    : Handler(Socket(a.family(), Socket::TCP)) 
  {
    socket().bind(a);
    socket().listen();
  }


  Socket& socket() { return static_cast<Socket&>(resource()); }

  // When data is available for reading, accept the connection
  // and spawn a new handler.
  Result on_read() {
    Socket c = socket().accept();
    
    // FIXME: Create a new handler for for socket and register
    // the connection with the reactor loop.

    return CONTINUE;
  }
};



void
register_handler(Resource_set& rs, Handler& h) {
  rs.insert(h.fd());
}

void
register_handlers(Resource_set& rs, const Handler_map& hs) {
  for (const auto& x : hs)
    register_handler(rs, *x.second);
}


int 
main(int argc, char* argv[]) {

  // Configure the switch address.
  Address addr(Ipv4_addr::any, 9000);

  Terminator term(0);
  Acceptor acc(addr);

  // Register default service handlers.
  // FIXME: nlogn lookup for file descriptors is slow.
  Handler_map handlers;
  handlers.insert({term.fd(), &term});
  handlers.insert({acc.fd(), &acc});

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
    int nev = s();

    // The close set is the set of handlers whid
    Resource_set close;

    // Handle events
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

    // Close any handlers that need to be destroyed.
    for (int i = 0; i < nev; ++i) {
      if (close.test(i))
        handlers[i]->close();
    }
  
  }

  // TODO: Actually clean up resources.


  /*
  // Create the accept socket.
  Address addr(Ipv4_addr::any, 9001);
  Socket acc(Socket::IP4, Socket::TCP);
  acc.bind(addr);
  acc.listen();




  int maxfd = acc.fd() + 1;
  while (1) {
    Resource_set reads;
    Resource_set writes;
    
    // Build the read/write sets
    reads.insert(0);
    reads.insert(acc);
    for (const auto& x : cm)
      reads.insert(x.second.socket());

    // Select which files are available and which are not.
    std::cout << maxfd << " -- " << reads.test(0) << '\n';
    Selector s(maxfd, reads, writes);
    s();

    // Read from stdin to see if it's been closed.
    if (reads.test(0)) {
      char c;
      if (::read(0, &c, 1) <= 0)
        break;
    }

    // Check the accept first
    if (reads.test(acc)) {
      // Accept the connection, and spin up a new service.
      Socket s = acc.accept();
      int fd = s.fd();
      cm.emplace(fd, std::move(s));

      // Update the maximum fd.
      maxfd = fd + 1;
    }

    // Iterate over over the connections to see if any can
    // actually be read.
    for (auto& x : cm) {
      Connection& c = x.second;
      bool erase = false;

      // Maybe cause a read.
      if (reads.test(c.socket()))
        erase |= c.read();

      // Maybe cause a write.
      if (reads.test(c.socket()))
        erase |= c.write();
      
      // Cleanup the handler.
      if (erase)
        cm.erase(x.first);
    }
  }

  */

  return 0;
}
