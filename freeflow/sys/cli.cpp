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

using Named_argument = Arguments::String_map::value_type;

Named_argument
parse_flag(const std::string& arg) {
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

  // Parse the name from the flag. If the flag is of the from
  // f=x, this parses out f. If the '=' is not present, this
  // returns the name f.
  std::string name;
  std::size_t n = arg.find_first_of('=', p);
  if (n != arg.npos)
    name = arg.substr(p, n - p);
  else
    return {arg.substr(p), "true"};

  // Parse the value. In a flag of the form f=x, this is everything
  // past the '='. If the value is empty, return as if it were "null".
  std::string value = arg.substr(n + 1);
  if (value.empty())
    return {std::move(name), ""};
  else
    return {std::move(name), std::move(value)};
}

/// Return an environment variable name constructed from the given
/// prefix and parameter name.
inline std::string
make_env_var(const std::string& pre, const Parameter& parm) {
  return pre + "_" + toupper(parm.name());
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
        args.initial_[n->second->name()] = x.second;
      }
    }
    else
      args.listed_.push_back(argv[i]);
  }
}

/// Parse the enviornment, constructing arguments from those environment
/// variables listed in the parameter specification. This effectively
/// filters the environment to find only those variables that the 
/// configuration is interested in.
void 
parse_env(const Parameters& parms, Arguments& args, const char* prefix) {
  std::string pre = toupper(prefix);
  for (auto parm : parms.parms_) {
    std::string var = make_env_var(pre, parm);
    if (char* p = getenv(var.c_str()))
      args.initial_.emplace(parm.name(), p);
  }
}

/// Iterate through the list of parameters and check the environment
/// to see if it contains any of them. Any arguments found in the
/// environment are added to the (initial) arguments map.
void 
parse_env(const Parameters& parms, Arguments& args, const std::string& prefix) {
  parse_env(parms, args, prefix.c_str());
}

void
check_type(const Parameter& parm, Arguments& args, const std::string& val) {
  Value v = parm.type()(val);
  if(!v) 
    std::cerr << "error: argument '" << parm.name() << "' has invalid type\n";
  else if(args.named_.count(parm.name()))
    args.named_[parm.name()] = v;
  else 
    args.named_.emplace(parm.name(), v); 
}

void check_args(const Parameters& parms, Arguments& args) {
  for (auto parm : parms.parms_) {
  // for (const Parameter& parm : parms.parms_) {
    // // Check for default arguments
    // if (parm.has_default() and args.has_option(parm.name()))
    //   check_type(parm, args, parm.default_argument());

    // // Check for required values.      
    // else if (parms.is_required() and not args.has_option(parm.name()))
    //   std::cerr << "error: no argument provided for required parameter '" 
    //             << parm.name() << "'\n";
    
    // // Otherwise, if the parameter has a value, just check its type.
    // else if (args.has_option(parm.name()))
    //   check_type(parm, args, args.option(parm.name()));
    bool r = true;
    Value v;

    // This checks whether an argument 
    if (args.has_initial(parm))
      check_type(parm, args, args.get_initial(parm.name()));
    
    // An argument for the parameter was not provided. This covers the rest of
    // the cases where 'which' is DEFAULT. A valid default value must exist for
    // the parameter
    else if (parm.init().which == cli::DEFAULT) 
      check_type(parm, args, parm.init().value);

    // An argument for the parameter was not provided. In this case 'which'
    // may be OPTIONAL or REQUIRED. Cases where 'which' is OPTIONAL can be
    // disregarded since no argument was provided.
    else if(parm.init().which == cli::REQUIRED)
      std::cerr << "error: no argument provided for required parameter '" 
                << parm.name() << "'\n";
    
  }
}


void parse(const Parameters& parms,
           Arguments& args, 
           int argc, 
           char* argv[], 
           const char* prefix) 
{
  parse_env(parms, args, prefix);
  parse_args(parms, args, argc, argv);

  check_args(parms, args);
}


} // namespace cli
} // namespace freeflow

