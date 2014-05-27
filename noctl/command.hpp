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

#ifndef FREEFLOW_COMMAND_HPP
#define FREEFLOW_COMMAND_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <map>

#include <freeflow/sys/json.hpp>
#include "parameter.hpp"


namespace cli {

using String_map = std::map<std::string, std::string>;
using String_list = std::vector<std::string>;

struct Command {
  virtual void run(String_map&, String_list) = 0;
  String_map params;
  String_map alias;
  std::string helptext;  
  
  static std::map<std::string, Command *> commands;
};



struct Help : Command {
  void run(String_map&, String_list);
  std::string help();
};

struct Add : Command {
  void run(String_map&, String_list);
};



//addCommand("add","Help text",add_run);

} // namespace cli

#include "command.ipp"

#endif
