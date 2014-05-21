// Copyright (c) 2013-2014 Flowgrammable, LLC.
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

// The echo server is a simple service that prints text messages from
// a client. The address and port of the client are printed.
//
// For now, this simply runs over TCP/IPv4.

#include <iostream>

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/acceptor.hpp>
#include <freeflow/sys/reactor.hpp>

using namespace std;
using namespace freeflow;

struct Echo_server : Socket_handler {
  Echo_server(Reactor&, Socket&&);

  bool on_open();
  bool on_close();
  bool on_read();
};

/// Note that this constructor can be inherited from Socket_handler.
Echo_server::Echo_server(Reactor& r, Socket&& s)
  : Socket_handler(r, READ_EVENTS, move(s)) { }

bool
Echo_server::on_open() {
  cout << "[...] connected\n";
  return true;
}

bool
Echo_server::on_close() { 
  cout << "[...] closed\n";
  return true;
}

bool
Echo_server::on_read() { 
  char buf[2048];
  System_result r = rc().recv(buf, 2048, 0);
  if (r.failed()) {
    cout << "[...] error\n";
    return false;
  }
  
  // Close the socket if we read 0 bytes.
  if (not r.value())
    return false;
  cout << "[...] message\n";
  
  return true;
}

using Echo_acceptor = Acceptor<Echo_server>;

int 
main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "usage: ./echo port\n";
    return -1;
  }
  Ip_port p = atoi(argv[1]);

  // Create the reactor.
  Reactor r;
  
  // Create and configure the reactor.
  Echo_acceptor a(r);
  a.listen(Address(Ipv4_addr::any, p), Socket::TCP);

  // Add the acceptor.
  r.add_handler(&a);

  r.run();
}
