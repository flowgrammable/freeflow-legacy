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

using Named_argument = Parsed_arguments::Argument_map::value_type;

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
    return {std::move(name), "null"};
  else
    return {std::move(name), std::move(value)};
}

// Parses the command line inputs into flags and positional arguments
Parsed_arguments
parse(int argc, char *argv[]) {
  Parsed_arguments args;
  for (int i = 0; i < argc; ++i) {
    if (argv[i][0] == '-')
      args.named.insert(parse_flag(argv[i]));
    else
      args.listed.push_back(argv[i]);
  }
  return args;
}

/// Return an environment variable name constructed from the given
/// prefix and parameter name.
inline std::string
make_env_var(const std::string& pre, const Parameter& parm) {
  return pre + "_" + toupper(parm.name());
}



} // namespace

Arguments
parse_args(const Parameters& parms, int argc, char* argv[]) {
  Parsed_arguments pa = parse(argc, argv);

  // FIXME: Post-process the parsed arguments, creating the new
  // argument set.

  return {};
}

/// Do stuff...
Arguments 
parse_env(const Parameters& parms, const char* prefix){
  std::string pre = toupper(prefix);
  Parsed_arguments args;
  for (auto parm : parms.parms_) {
    std::string var = make_env_var(pre, parm);
    if (char* p = getenv(var.c_str()))
      args.named.emplace(parm.name(), p);
  }

  // FIXME: Postprocess parsed arguments, checking for required
  // values and instantiating default arguments.

  return {};
}

/// Iterate through the list of parameters and check the environment
/// to see if it contains any of them. Any arguments found in the
/// environment are added to the (named) arguments map.
Arguments 
parse_env(const Parameters& parms, const std::string& prefix){
  return parse_env(parms, prefix.c_str());
}



} // namespace cli
} // namespace freeflow

