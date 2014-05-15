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


// Holds a command-line flag. The flag is set when --flag_true is passed, and
// reset when --flag_false is passed. When used in CommandParser, the name
// and value act as a key-value pair. Upon construction, value will be set to
// the default value of the flag.
//
// Example usage:
//
// Flag("auto connect","auto-connect","no-auto-connect",true, "Whether to
//   automatically connect to the network");
//
// If the user typed "run --auto-connect", value for "auto connect" would
// be true, whereas is they typed "run --no-auto-connect", it would be false
struct Flag {
  string name;
  string flag_true;
  string flag_false;
  bool value;
  string help_text;
  
  Flag(string n,string f_true,string f_false,bool default_val,string help = "") :
    name(n), flag_true(f_true), flag_false(f_false), value(default_val),
    help_text(help){}
};



int main(){
  return 0;
}























