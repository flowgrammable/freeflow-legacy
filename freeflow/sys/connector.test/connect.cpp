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

// Read from standard input and send through the connection.
struct Reader : Resource_handler {
  Reader(Reactor& r, Echo_client& c)
    : Resource_handler(r, READ_EVENTS, 0), conn(c) { }

  // If there is no more data to read, indicate that we want
  // to terminate the reactor loop. Note that the read should
  // never be deferred.
  bool on_read(Reactor& r) {
    char buf[1024];
    System_result res = read(rc(), buf, 1024);
    if (res.completed()) {
      std::size_t n = res.value();
      if (n == 0) {
        std::cout << "* terminating\n";
        r.stop();
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

  Echo_client& conn;
};

using Echo_conector = Connector<Echo_client>;

int main() {

  Reactor r;

  // Create and add the service handlers.
  Echo_connector conn(r);
  Reader read(r, conn);
  r.add_handler(&conn);
  r.add_handler(&read);

  // Run the event loop.  
  r.run();
}
