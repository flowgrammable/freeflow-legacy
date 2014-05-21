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
#include <vector>
#include <map>
#include <assert.h>

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/json.hpp>

using namespace std;
using namespace freeflow;
using namespace json;

namespace cli {

using string_map = map<string, string>;
using string_list = vector<string>;

// Parses the command line inputs into flags and positional arguments
void parse (int argc, char *argv[], string_map &flags, string_list &pos_args) {
  for (int i = 0; i < argc; ++i) {
    string arg = argv[i];
    bool set_value = false;
    
    if(arg[0] == '-') {
      string name;
      string value;
      unsigned int pos = 0;
      
      // Get the name of the flag
      while (pos < arg.length() && arg[pos] != '=')
        name += arg[pos++];
      
      // If '=' is not used, assume a default value of true
      if(pos < arg.length() && arg[pos] == '=')
        set_value = true;
      else
        value = "true";
        
      // Get the value of the flag
      while (++pos < arg.length())
        value += arg[pos];
      
      // If they used '=' but didn't specify a value, assume "NULL"
      if (set_value && value == "")
        value = "NULL";
        
      flags[name] = value;
    }
    else {
      pos_args.push_back(arg);
    }
  }
}

} // namespace cli

using namespace cli;

int
main(int argc,char *argv[]) {
  string_map flags;
  string_list pos_args;
  
  parse(argc, argv, flags, pos_args);
  
  for (auto &f : flags) {
    cout << f.first << " is set to " << f.second << endl;
  }
  
  cout << endl << "Positional args" << endl << endl;
  
  for (auto &s : pos_args) {
    cout << s << endl;
  }
  
  return 0;
}

















