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
};

CLI_parser_exception::CLI_parser_exception(string err) {
  cout << "CLI parser exception: " << err << endl;
}

struct Data_type {
  Value (*to_json)(string & value);
  
  Data_type( Value (*json)(string &) );
  
  Data_type(){}
};

Data_type::Data_type( Value (*json)(string &) ) : to_json(json) {}

struct CLI_parser {
  vector<string> args;
  Object object;
  map<string, Data_type> types;
  
  void add_data_type(string & type, Value (*to_json)(string &) );
};

void
CLI_parser::add_data_type(string & name, Value (*json)(string &) ) {
  types[name] = Data_type(json);
}

Value string_to_json(string & s) {
  Value v(s);
  
  
  return v;
}


int
main(int argc,char *argv[]) {
  CLI_parser parser;
}























