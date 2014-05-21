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

// Very simple CLI exception handling class
// TODO: redirect output to stderr
struct CLI_parser_exception {
  CLI_parser_exception(string err);
  CLI_parser_exception(string err, int arg);
};

CLI_parser_exception::CLI_parser_exception(string err) {
  cout << "CLI parser exception: " << err << endl;
}

CLI_parser_exception::CLI_parser_exception(string err, int arg) {
  cout << "CLI parser exception for argument " + to_string(arg) + ": " <<
    err << endl;
}

struct CLI_parser {
  vector<string> pos_args;
  map<string, string> args;
  map<string, string> alias;
  
  bool parse(int argc, char *argv[]);
  void add_alias(string full, string alias_name);
};

bool
CLI_parser::parse(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    string arg = argv[i];
    string name;
    string value;
    unsigned int pos = 0;
    bool set_value = false;
    
    // Get the argument name
    while (pos < arg.length() && arg[pos] != '=') {
      name += arg[pos++];
    }
    
    // Check if they provided an explicit value for the flag.
    // If not, it should be set to a default of true.
    if (pos < arg.length() && arg[pos] == '=')
      set_value = true;
    else
      value = "true";
    
    // Get the argument value, if there is one
    while (++pos < arg.length()) {
      value += arg[pos];
    }
    
    // TODO: if set_value is true and they didn't specify a value after the '=',
    // should we throw an exception?
    
    if (arg[0] == '-') {
      if (arg.length() > 1) {
        if (arg[1] == '-') {
          // TODO: check if the value already exists in the map
          args[name] = value;
        }
        else {
          // The name is an alias for a longer name.
          // TODO: either throw a warning if it contains more than one character
          // or take everything after the first letter to be part of the value.
          
          string alias_name = name.substr(0,2);
          
          if (alias.count(alias_name) == 0) {
            throw CLI_parser_exception("Unrecognized flag alias",i);
          }
          else {
            args[alias[alias_name]] = value;
          }
        }
      }
      else {
        throw CLI_parser_exception("Expected flag name after '-'", i);
      }
    }
    else {
      pos_args.push_back(name);
    }
  }
  
  return true;
}

// Adds a short alias for a long command
void
CLI_parser::add_alias(string full, string alias_name) {
  alias[alias_name] = full;
}

int
main(int argc,char *argv[]) {
  CLI_parser parser;
  
  parser.add_alias("-output","-o");
  parser.parse(argc,argv);
  
  cout << "----Flags----" << endl;
  
  for (auto &p : parser.args) {
    cout << "'" << p.first << "' is set to '" << p.second << "'" << endl;
  }
  
  cout << endl << "----Positional Args----" << endl << endl;
  
  for (string &s : parser.pos_args) {
    cout << s << endl;
  }
}























