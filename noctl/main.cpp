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



#include <assert.h>

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/json.hpp>
#include <freeflow/sys/cli.hpp>

#include "command.hpp"

using namespace std;
using namespace freeflow;

int
main(int argc, char *argv[]) {
  cli::Parameters parms;
  parms.declare("flag, f", cli::Bool(), cli::REQUIRED, "Just a flag");
  parms.declare("number", cli::Real(), "42", "Just a number");
  parms.declare("name", cli::String(), "some value", "The name of something");
  parms.declare("config", cli::String(), cli::OPTIONAL, "The path to a configuration file");
  parms.declare("path", cli::String(), "*default path*", "Path to something");
  parms.declare("version", cli::Real(), cli::REQUIRED, "Version of something");

  cli::Arguments args;

  parse(parms, args, argc, argv, "flog");


  cout << "Parsed arguments:\n";
  for (auto& x : args.named)
    cout << x.first << " = " << x.second << '\n';



  // Command *c = nullptr;
  // Command::commands["hello"] = c;

  return 0;
}
