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

#include "parameter.hpp"

namespace cli {

// -------------------------------------------------------------------------- //
// Parsing

std::pair<std::string, std::string>
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
void
parse(int argc, char *argv[], String_map& opts, String_list& args) {
  for (int i = 0; i < argc; ++i) {
    if (argv[i][0] == '-')
      opts.insert(parse_flag(argv[i]));
    else
      args.push_back(argv[i]);
  }
}

Argument_map
parse(const Parameter_set&, int argc, char* argv[]) {
  return {};
}

} // namespace cli

