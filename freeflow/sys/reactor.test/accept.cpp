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

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/reactor.hpp>

using namespace freeflow;

struct Echo_server : Socket_handler {
  Echo_server(Reactor& r, Socket&& s)
    : Socket_handler(r, READ_EVENTS, std::move(s)) { }

  bool on_read() {
    char buf[1024];
    System_result res = read(rc(), buf, 1024);
    if (res.failed()) {
      std::cout << "* error reading socket\n";
      return false;
    } else if (res.value() == 0) {
      std::cout << "* connection closed\n";
      return false;
    } else {
      buf[res.value()] = 0;
      std::cout << buf;
      return true;
    }
  }
};

struct My_acceptor : Socket_handler {
  My_acceptor(Reactor& r, const Address& a)
    : Socket_handler(r, READ_EVENTS, Socket::IP4, Socket::TCP) 
  { 
    if (Trap err = rc().bind(a))
      throw err.code();
    if (Trap err = listen(rc()))
      throw err.code();
  }

  // Acccept a connection and spin up a new service.
  bool on_read() {
    Socket s = accept(rc());
    std::cout << "* accepted connection\n";

    reactor().new_handler<Echo_server>(std::move(s));

    return true;
  }
};

int main() {
  Reactor r;
  My_acceptor c(r, Address(Ipv4_addr::any, 9876));
  r.add_handler(&c);
  r.run();
}
