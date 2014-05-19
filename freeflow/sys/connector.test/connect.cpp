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
#include <freeflow/sys/connector.hpp>
#include <freeflow/sys/reactor.hpp>

using namespace freeflow;

// The echo client is extremely thin...
struct Echo_client : Socket_handler {

  Echo_client(Reactor& r)
    : Socket_handler(r, NO_EVENTS) { }

  bool on_open() {
    std::cout << "* init client\n";
    return true;
  }

  bool on_close() {
    std::cout << "* end client\n";
    return true;
  }
};

// Read from standard input and send through the connection.
struct Input_reader : Resource_handler {
  Input_reader(Reactor& r, Echo_client& c)
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
        reactor().stop();
        return true;
      } else {
        buf[n] = 0;
        send(conn.rc(), &buf[0], n);
      }
      return true;
    } else {
      perror("error");
      return false;
    }
  }

  Echo_client& conn;
};

using My_connector = Connector<Echo_client>;

int main() {
  Reactor r;

  // The echo client maintains the client socket.
  Echo_client client(r);

  // // The reader accepts terminal input.
  Input_reader reader(r, client);

  // Establishes the asynchronous connection.
  My_connector conn(r);
  conn.connect(&client, Address(Ipv4_addr::loopback, 9876), Socket::TCP);

  // Add the reader and run the event loop.
  r.add_handler(&reader);
  r.run();
}
