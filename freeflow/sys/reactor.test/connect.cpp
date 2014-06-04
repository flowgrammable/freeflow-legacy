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

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/reactor.hpp>

using namespace freeflow;

struct Connection : Socket_handler {
  Connection(Reactor& r, const Address& a)
    : Socket_handler(r, WRITE_EVENTS, Socket::IP4, Socket::TCP) 
    {
      set_nonblocking(rc());
      System_result res = connect(rc(), a);
      if (res.failed())
        throw std::runtime_error("connection");
    }

  // Print a connection message and exit.
  //
  // FIXME: Stop responding to write events after being accepted.
  // We don't need to keep calling this function.
  bool on_write() {
    // FIXME: Actually check that connection has succeeded.
    std::cout << "* connected\n";

    // Don't receive write events any more.
    reactor().unsubscribe_events(this, WRITE_EVENTS);
    return true;    
  }

  bool connected = false;
};

// Read from standard input and send through the connection.
struct Reader : Resource_handler {
  Reader(Reactor& r, Connection& c)
    : Resource_handler(r, READ_EVENTS, 0), conn(c) { }

  // If there is no more data to read, indicate that we want
  // to terminate the reactor loop. Note that the read should
  // never be deferred.
  bool on_read() {
    char buf[1024];
    System_result res = read(rc(), buf, 1024);
    if (res.completed()) {
      std::size_t n = res.value();
      if (n == 0) {
        std::cout << "* terminating\n";
        reactor().stop();
        return false;
      }
      else {
        buf[n] = 0;
        send(conn.rc(), &buf[0], n);
      }
      return true;
    } else {
      perror("error");
      return false;
    }
  }

  Connection& conn;
};

int main() {

  Reactor r;

  // Create and add the service handlers.
  Connection conn(r, Address(Ipv4_addr::loopback, 9876));
  Reader read(r, conn);
  r.add_handler(&conn);
  r.add_handler(&read);

  // Run the event loop.  
  r.run();
}
