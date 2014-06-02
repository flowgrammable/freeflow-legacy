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

#include "cli.hpp"

namespace freeflow {
namespace cli {
namespace {

using Named_argument = Arguments::Initial_args_map::value_type;

Named_argument
parse_flag(const std::string& arg) {
  Source src = COMMAND_LINE;
  // If the argument is only '-', that's an error
  if (arg.size() == 1)
    throw std::runtime_error("parse error");

  // Make sure that p points to the first non-flag character.
  std::size_t p = 1;
  if (arg[p] == '-')
    ++p;

  // If the flag is "--", that's an error.
  if (p == arg.size())
    throw std::runtime_error("parse error");

  // Parse the name from the flag. If the flag is of the form
  // f=x, this parses out f. If the '=' is not present, this
  // returns the name f.
  std::string name;
  std::size_t n = arg.find_first_of('=', p);
  if (n != arg.npos)
    name = arg.substr(p, n - p);
  else
    return {arg.substr(p), Initial_argument("true", src)};

  // Parse the value. In a flag of the form f=x, this is everything
  // past the '='. If the value is empty, return as if it were "null".
  std::string value = arg.substr(n + 1);
  if (value.empty())
    return { std::move(name), Initial_argument("", src) };
  else
    return { std::move(name), Initial_argument(std::move(value), src) };
}


} // namespace


/// Parse the given command line arguments.
///
/// FIXME: Improve error handling. If an error occurs, the program
/// should probably stop running after diagnosing all errors.
void
parse_args(const Parameters& parms, Arguments& args, int argc, char* argv[]) {
  for (int i = 0; i < argc; ++i) {
    if (argv[i][0] == '-') {
      Named_argument x = parse_flag(argv[i]);
      if (parms.map_.count(x.first) == 0) {
        std::cerr << "error: unrecognized parameter '" << x.first << "'\n";
      } else {
        auto n = parms.map_.find(x.first);
        args.set_initial(*n->second, x.second);
      }
    }
    else
      args.set_listed(argv[i]);
  }
}

/// Parse the enviornment, constructing arguments from those environment
/// variables listed in the parameter specification. This effectively
/// filters the environment to find only those variables that the 
/// configuration is interested in.
void 
parse_env(const Parameters& parms, Arguments& args, const char* prefix) {
  Source src = ENVIRONMENT;
  std::string pre = toupper(prefix);
  for (const auto& parm : parms) {
    std::string var = make_env_var(pre, parm.name());
    if (char* p = getenv(var.c_str()))
      args.set_initial(parm, Initial_argument(p,src));
  }
}

/// Iterate through the list of parameters and check the environment
/// to see if it contains any of them. Any arguments found in the
/// environment are added to the (initial) arguments map.
void 
parse_env(const Parameters& parms, Arguments& args, const std::string& prefix) {
  parse_env(parms, args, prefix.c_str());
}

bool
check_type(const Parameter& parm, Arguments& args, const std::string& val) {
  args.set_named(parm.name(), parm.type()(val));
  if (args.get_named(parm.name()).type() == json::Value::ERROR)
    return false;
  else
    return true;
}

bool 
check_args(const Parameters& parms, const Command& cmd, Arguments& args) {
  bool r = true;
  // for (const std::string& parm_name: cmd.parameters) {
  for (const auto& parm : parms) {
    // An argument for the parameter was provided. In this case 'which' may be
    // OPTIONAL, REQUIRED, or DEFAULT
    // Parameter* parm = parms.map_.find(parm_name)->second;
    if (args.has_initial(parm.name()))
      r &= check_type(parm, args, args.get_initial_value(parm.name()));
    
    // An argument for the parameter was not provided. This covers the rest of
    // the cases where 'which' is DEFAULT. A valid default value must exist for
    // the parameter
    else if (parm.has_default()) {
      args.set_initial(parm, Initial_argument(parm.default_argument(), FROM_DEFAULT));
      r &= check_type(parm, args, parm.default_argument());
    }

    // An argument for the parameter was not provided. In this case 'which'
    // may be OPTIONAL or REQUIRED. Cases where 'which' is OPTIONAL can be
    // disregarded since no argument was provided.
    else if (parm.is_required()) {
      args.set_initial(parm, Initial_argument("", NOT_PROVIDED));
      args.set_named(parm.name(), Error(json::REQUIRED_ERROR, 0));
      r &= false;
    }
  }
  return r;
}

bool 
parse(const Parameters& parms,
      const Commands& cmds,
      Arguments& args, 
      int argc, 
      char* argv[], 
      const char* prefix) 
{
  parse_env(parms, args, prefix);
  parse_args(parms, args, argc, argv);

  if (args.get_listed_size() < 2) {
    std::cerr << "error: a command must be provided\n";
    return false;
  }

  std::string cmd_name = args.get_listed(1).as_string();

  if (!cmds.count(cmd_name)) {
    std::cerr << "error: command not recognized\n";
    return false;
  }
  
  // FIXME: If this returns null, it will crash.
  Command* cmd = cmds.find(cmd_name)->second;

  bool result = check_args(parms, *cmd, args);

  if (!result){ 
    args.display_errors(*cmd, prefix);
  }

  return result;
}

// -------------------------------------------------------------------------- //
// Commands

Help_command::Help_command(Commands& cmds)
  : Command("help", "Print help information on a topic")
  , cmds_(cmds)
{ }

void
Help_command::help() {
  for (auto p : cmds_)
    std::cout << p.first << ": " << p.second->help() << '\n';
}

void
Help_command::help(const Command& cmd) {
  std::cout << cmd.help() << '\n';
  for (const auto& p : cmd.parms())
    std::cout << p.doc() << '\n';
}

bool
Help_command::run(const Arguments& args) {
  // TODO: What if the user calls "help cmd1 cmd2"?
  if (args.get_listed_size()) {
    // FIXME: Verify that the argument is actually a string.
    if (Command* cmd = cmds_[args.get_listed(0).as_string()]) {
      help(*cmd);
    } else {
      std::cerr << "error: no such command '" << "'\n";
      return false;
    }
  } else {
    help();
  }
  return true;
}

} // namespace cli
} // namespace freeflow

