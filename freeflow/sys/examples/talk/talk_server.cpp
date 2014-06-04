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

struct Talk_server : Socket_handler {
  Talk_server(Reactor&, Socket&&);

  bool on_open();
  bool on_close();
  bool on_read();

  void send_all(const int&);
  std::ostream& log();
  std::list<Socket*> clients;
  Buffer buf;
};

/// Note that this constructor can be inherited from Socket_handler.
Talk_server::Talk_server(Reactor& r, Socket&& s)
  : Socket_handler(r, READ_EVENTS, move(s)) 
  , buf(2048) { }

bool
Talk_server::on_open() {
  std::cout << "connected " <<  bracket(rc().peer) << "\n";
  clients.push_back(&rc());
  return true;
}

bool
Talk_server::on_close() { 
  std::cout << "disconnected " << bracket(rc().peer) << "\n";
  clients.remove(&rc());
  return true;
}

bool
Talk_server::on_read() { 
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

  send_all(n);
  
  return true;
}

std::ostream&
Talk_server::log() {
  return std::cout << bracket(rc().peer) << ' ';
}

void
Talk_server::send_all(const int& n) {
  for (const auto& s : clients) {
    send(*s, &buf[0], n);
  }
}

// struct Talk_server_acceptor : Socket_handler {
//   Talk_server_acceptor(Reactor& r, const Address& a)
//     : Socket_handler(r, READ_EVENTS, Socket::IP4, Socket::TCP) 
//   { 
//     if (Trap err = rc().bind(a))
//       throw System_error(err.code());
//     if (Trap err = listen(rc()))
//       throw System_error(err.code());
//   }

//   // Acccept a connection and spin up a new service.
//   bool on_read() {
//     Socket s = accept(rc());
//     std::cout << "* accepted connection\n";

//     reactor().new_handler<Talk_server>(std::move(s));

//     return true;
//   }
// };

using Talk_acceptor = Acceptor<Talk_server>;

int 
main(int argc, char* argv[]) {
  const char* prefix = "flog";

  Parameters parms;
  parms.declare("port, p", cli::Int_typed(), cli::REQUIRED, "The port of the talk server");

  // Initialize the parse state
  Parse_state ps(argc, 1, argv);
  if (ps.argc < 2) {
    std::cerr << "error: a port number must be provided\n";
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
  if (ps.current > 2) {
    std::cerr << "error: invalid argument provided\n";
    return -1;
  }

  // At this point we know the port was provided and nothing else
  Ip_port p = program_args.get_named_value("port").as_int();
  
  // Create the reactor.
  Reactor r;
  
  // Create and configure the reactor.
  Talk_acceptor a(r);
  a.listen(Address(Ipv4_addr::any, p), Socket::TCP);

  // Add the acceptor.
  r.add_handler(&a);

  r.run();
}
