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
#include <fstream>
#include <iterator>
#include <string>

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/json.hpp>

using namespace std;
using namespace freeflow;


struct Config {
  Address ctrl_addr {Address::IP4, "127.0.0.1", 9001};
};

enum Command {
  VERSION_CMD,
  HELP_CMD,
  RAW_CMD
};

// Print version information.
int 
version() {
  std::cout << "noctl v0.1\n";
  std::cout << "Copyright (c) 2013-2014 Flowgrammable.org\n";
  return 0;
}

int 
help() {
  std::cout << "noctl [prog-opts] command [cmd-opts] [cmd-args]\n";
  return 0;
}

int
raw(int arg, int argc, char* argv[]) {
  if (arg == argc) {
    std::cerr << "usage: noctl raw <json-file>\n";
    return -1;
  }

  string buf;
  try {
    // Read the json-file into a buffer.
    ifstream f(argv[arg]);
    istreambuf_iterator<char> iter(f);
    istreambuf_iterator<char> end;
    buf = string(iter, end);
  } catch (...) {
    std::cerr << "error: could not read file\n";
    return -1;
  }

  try {
    Config conf;
    Socket s(Socket::IP4, Socket::TCP);
    System_error err = connect(s, conf.ctrl_addr);
    std::cout << "HERE: " << err.code() << '\n';

    int n = s.write(buf.c_str(), buf.size());
    std::cerr << "sent " << n << " bytes\n";
  } catch (...) {
    std::cerr << "error: could not send command\n";
    return  -1;
  }

  return 0;
}

int 
main(int argc, char* argv[]) {
  // Parse command line arguments.
  //
  // FIXME: This is totally broken. Parsing options occurs in 3
  // pases. First, the program options (applies to all commands),
  // the the command, then the command options and arguments (which 
  // actually depend on the command). Note that only one command
  // is permitted.
  Command cmd = HELP_CMD;
  int arg = 1;
  for (; arg < argc; ++arg) {
    string s = argv[arg++];
    if (s == "version") {
      cmd = VERSION_CMD; 
      break;
    } else if (s == "help") {
      cmd = HELP_CMD;
      break;
    } else if (s == "raw") {
      cmd = RAW_CMD;
      break; 
    } else {
      std::cerr << "error: unknown command\n";
      help();
      return -1;
    }
  }

  switch (cmd) {
  case VERSION_CMD: return version();
  case HELP_CMD: return help();
  case RAW_CMD: return raw(arg, argc, argv);
  }

}

