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

#include <iostream>

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/acceptor.hpp>
#include <freeflow/sys/connector.hpp>
#include <freeflow/sys/reactor.hpp>
#include <freeflow/sys/cli.hpp>

#include "service.hpp"

using namespace std;
using namespace freeflow;
using namespace cli;

/// The talk client represents a client that is connected to a 
/// talk server.
struct Talk_client : Socket_handler {

  Talk_client(Reactor& r, Socket&& s)
    : Socket_handler(r, READ_EVENTS, std::move(s)) { }

  bool 
  on_open() {
    std::cout << "* init client\n";
    return true;
  }

  bool 
  on_close() {
    std::cout << "* end client\n";
    return true;
  }

  bool 
  on_read() { 
    // Read data into the buffer.
    Buffer buf(2048);
    System_result r = rc().read(buf);

    // Make sure that this didn't fail.
    if (r.failed()) {
      log() << "read error: '" << strerror(errno) << "'\n";
      return false;
    }

    int n = r.value();
    
    // Close the socket if we read 0 bytes.
    if (n == 0)
      return false;
    
    // Log the message. Write a newline if needed.
    log() << buf.data();
    if (buf[n-1] != '\n')
      std::cout << '\n';

    return true;
  }

  std::ostream&
  log() {
    return std::cout << bracket(rc().peer) << ' ';
  }

};

// Read from standard input and send through the connection.
struct Input_reader : Resource_handler {
  Input_reader(Reactor& r)
    : Resource_handler(r, READ_EVENTS, 0), conn(nullptr) { }

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
        send(conn->rc(), &buf[0], n);
      }
      return true;
    } else {
      perror("error");
      return false;
    }
  }

  Talk_client* conn;
};

/// The Talk factory is responsible for creating the Talk connection.
/// It binds the created client to the input reader.
struct Talk_factory {
  Talk_factory(Input_reader& r)
    : reader(r) { }

  Talk_client* operator()(Reactor& r, Socket&& s) {
    Talk_client* c = new Talk_client(r, std::move(s));
    reader.conn = c;
    return c;
  }

  Input_reader& reader;
};

using Talk_connector = Connector<Talk_client, Talk_factory>;

int 
main(int argc, char* argv[]) {
  const char* prefix = "flog";

  Parameters parms;
  parms.declare("host, h", cli::Int_typed(), cli::REQUIRED, "The host of the talk client");
  parms.declare("port, p", cli::Int_typed(), cli::REQUIRED, "The port of the talk client");

  // Initialize the parse state
  Parse_state ps(argc, 1, argv);
  if (ps.argc < 3) {
    std::cerr << "error: a host and port number must be provided\n";
    return -1;
  }

  // Parse for program options
  cli::Arguments program_args;
  parse_env(parms, program_args, prefix);
  parse_keyword_args(parms, program_args, ps);

  // Check program args
  if (not check_args(parms, program_args)) { 
    program_args.display_errors(prefix);
    return -1;
  }

  // Make sure no commands or positional arguments were provided
  if (ps.current > 3) {
    std::cerr << "error: invalid argument provided\n";
    return -1;
  }

  // At this point we know the port and host were provided and nothing else
  Ip_port p = program_args.get_named_value("port").as_int();
  Address host(Address_info::IP4, program_args.get_named_value("host").as_string(), p);
  
  // Create the reactor.
  Reactor r;
  
  // The reader accepts terminal input.
  Input_reader reader(r);
  r.add_handler(&reader);

  // Establishes the asynchronous connection.
  Talk_connector conn(r, reader);
  conn.connect(host, Socket::TCP);

  r.run();
}
