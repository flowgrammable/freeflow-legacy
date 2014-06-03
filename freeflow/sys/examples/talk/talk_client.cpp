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

struct Talk_client : Socket_handler {
  Talk_client(Reactor&, Socket&&);

  bool on_open();
  bool on_close();
  bool on_read();

  std::ostream& log();

  Buffer buf;
};

/// Note that this constructor can be inherited from Socket_handler.
Talk_client::Talk_client(Reactor& r, Socket&& s)
  : Socket_handler(r, READ_EVENTS, move(s)) 
  , buf(2048) { }

bool
Talk_client::on_open() {
  log() << "connected\n";
  return true;
}

bool
Talk_client::on_close() { 
  log() << "disconnected\n";
  return true;
}

bool
Talk_client::on_read() { 
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
Talk_client::log() {
  return std::cout << bracket(rc().peer) << ' ';
}

using Talk_client_acceptor = Acceptor<Talk_client>;




int 
main(int argc, char* argv[]) {
  bool success = true;
  Parameters parms;
  parms.declare("host, h", cli::Int_typed(), cli::REQUIRED, "The port of the talk client");
  parms.declare("port, p", cli::Int_typed(), cli::REQUIRED, "The port of the talk client");

  // Initialize the parse state
  Parse_state ps(argc, 1, argv);
  if (ps.argc < 3) {
    std::cerr << "error: a host and port number must be provided\n";
    return -1;
  }

  // Initialize the program arguments
  cli::Arguments program_args;

  // Parse the environment for program options
  const char* prefix = "flog";
  parse_env(parms, program_args, prefix);

  // Parse the command-line for program options
  parse_keyword_args(parms, program_args, ps);

  // Check program args
  success &= check_args(parms, program_args);

  if (!success){ 
    program_args.display_errors(prefix);
    return -1;
  }

  // Make sure no commands or positional arguments were provided
  if (ps.current != ps.argc) {
    std::cerr << "error: invalid argument provided\n";
    return -1;
  }

  // At this point we know the port was provided and nothing else
  Ip_port p = program_args.get_named_value("port").as_int();
  
  // Create the reactor.
  Reactor r;
  
  // Create and configure the reactor.
  Talk_client_acceptor a(r);
  a.listen(Address(Ipv4_addr::any, p), Socket::TCP);

  // Add the acceptor.
  r.add_handler(&a);

  //r.run();
}
