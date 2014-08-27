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

struct Add_command : cli::Command {
  Add_command() 
    : Command("add", "Add something to something else")
  {
    declare("name", cli::String_typed(), cli::REQUIRED, "The name of the thing added");
    declare("path", cli::String_typed(), cli::REQUIRED, "The path to the thing added");
    declare("config", cli::String_typed(), cli::REQUIRED, "Path to a configuration file");
    declare("version", cli::String_typed(), cli::REQUIRED, "The version of the thing added");
    declare("flag", cli::Bool_typed(), cli::REQUIRED, "A mysterious flag");
  }

  bool run(const cli::Arguments& args) { 
    std::cout << "Adding...\n";
    return true; 
  }
};

struct Del_command : cli::Command {
  Del_command() 
    : Command("del", "Remove something from something else")
  {
    declare("name", cli::String_typed(), cli::REQUIRED, "The name of the thing removed");
    declare("path", cli::String_typed(), cli::REQUIRED, "The path to the thing being removed");
    declare("config", cli::String_typed(), cli::REQUIRED, "Path to a configuration file");
  }

  bool run(const cli::Arguments& args) {
    std::cout << "Removing...\n";
    return true;
  }
};

int
main(int argc, char *argv[]) {
  cli::Parameters parms;
  parms.declare("flag, f", cli::Bool_typed(), cli::REQUIRED, "Just a flag");
  parms.declare("number", cli::Real_typed(), "42", "Just a number");
  parms.declare("name", cli::String_typed(), "some value", "The name of something");
  parms.declare("config", cli::String_typed(), cli::OPTIONAL, "The path to a configuration file");
  parms.declare("path", cli::String_typed(), "*default path*", "Path to something");
  parms.declare("version, v", cli::Real_typed(), cli::REQUIRED, "Version of something");

  // Create commands.
  cli::Commands cmds;
  cmds.declare<Add_command>();
  cmds.declare<Del_command>();

  // Initialize the parse state
  cli::Parse_state ps(argc, 1, argv);
  if (ps.argc == 1) {
    std::cerr << "error: a command must be provided\n";
    return -1;
  }

  // Initialize the program arguments
  cli::Arguments program_args;

  // Parse the environment for program options
  const char* prefix = "flog";
  parse_env(parms, program_args, prefix);

  // Parse the command-line for program options up to the command
  parse_keyword_args(parms, program_args, ps);

  // Check program args
  if (not check_args(parms, program_args))
    program_args.display_errors(prefix);

  // Make sure a command name was provided
  if (ps.current == ps.argc) {
    std::cerr << "error: a command must be provided\n";
    return -1;
  }
  
  // Make sure the command exists
  std::string cmd_name = ps.argv[ps.current];
  if (!cmds.count(cmd_name)) {
    std::cerr << "error: command not recognized\n";
    return -1;
  }

  // FIXME: If this returns null, it will crash.
  cli::Command* cmd = cmds.find(cmd_name)->second;
  cli::Arguments command_args;
  
  // Parse command args
  parse_args(cmd->parms(), command_args, ps);

  // Check command args
  if (not check_args(cmd->parms(), command_args)) {
    command_args.display_errors(prefix);
    return -1;
  }

  // FIXME: Return from this?
  cmd->run(command_args);
}
