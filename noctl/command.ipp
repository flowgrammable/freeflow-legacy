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

namespace freeflow {
namespace cli {

inline
Command::Command(const std::string& n, 
                 const Run& r, 
                 const Parms& p, 
                 const std::string& h)
  : name(n), run(r), parameters(p), helptext(h) { }

inline void
Commands::declare(const std::string& n, 
                  const Run& r, 
                  const Parms& p, 
                  const std::string& h)
{
  commands.emplace(n, Command(n, r, p, h));
}

inline void 
Commands::help() const{
  for(auto command : commands) {
    std::cout << command.first << "\t\t"
    << command.second.helptext << "\n";
  }
}

inline void 
Commands::help(const std::string& cmd) const{
  auto command = commands.find(cmd);
  std::cout << command->first << "\t\t"
  << command->second.helptext << "\n";
}

inline void
Add::operator()(const Arguments& args) const {
  std::cout << "Ran add" << "\n";
}

inline void
Remove::operator()(const Arguments& args) const {
  std::cout << "Ran remove" << "\n";
}

inline void
run(const Commands& cmds, const Arguments& args) {
  if(args.listed.size()<2)
    std::cerr << "error: a command must be provided" << "\n";
  else if(args.listed[1] == "help"){
    if (args.listed.size()>2)
      cmds.help(args.listed[2]);
    else
      cmds.help();      
  }
  else 
    cmds.commands.find(args.listed[1])->second.run(args);
}


} // namespace cli
} // namespace freeflow