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
#include <assert.h>

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/json.hpp>

using namespace std;
using namespace freeflow;

struct CLI_parser_exception {
  CLI_parser_exception(string err);
};

CLI_parser_exception::CLI_parser_exception(string err) {
  cout << "CLI parser exception: " << err << endl;
}

struct CLI_parser_rule {
  string format;
  
  bool match(string & str,vector<pair<string,string> > args);
  
  CLI_parser_rule(string rule);
};

CLI_parser_rule::CLI_parser_rule(string rule) : format(rule) {
  // Make sure all square brackets are closed
  bool in_bracket = false;
  
  for (char c : format) {
    if (c == '[') {
      if (not in_bracket) {
        in_bracket = true;
      }
      else {
        throw CLI_parser_exception("Too many opening square brackets ('[')"
          " in parser rule: " + format);
      }
    } else if (c == ']') {
      if (in_bracket) {
        in_bracket = false;
      }
      else {
        throw CLI_parser_exception("Too many closing square brackets (']')"
          " in parser rule: " + format);
      }
    }
  }
  
  if (in_bracket){
    throw CLI_parser_exception("Expected closing square bracket (']')"
      " in parser rule: " + format);
  }
}

bool CLI_parser_rule::match(string & str,vector<pair<string,string> > args) {
  int pos_f = 0;
  int pos_s = 0;
  string name;
  string var_value;
  bool var = false;
  
  
  while (pos_s < (int)str.length()) {
    if (!var) {
      if (format[pos_f] == '[') {
        pos_f++;
        name = "";
        var_value = "";
        var = true;
        
        bool matched = false;
        
        while (pos_f < (int)format.length()) {
          if (format[pos_f] == ']') {
            matched = true;
            pos_f++;
            break;
          }
          else{
            name += format[pos_f++];
          }
        }
        
        if (!matched) {
          throw "CLI_parser_rule Error: unmatched bracket in rule "+format;
        }
        
        cout << "Next: " << format[pos_f] << endl;
        
        //pos_f++;
      }
    }
    
    if (var) {
      //if(pos_f+1 < format.length()){
        if (str[pos_s] == format[pos_f]) {
          var = false;
          cout << "Var " << name << " = " << var_value << endl;
          args.push_back(pair<string,string>(name,var_value));
        }
        else {
          var_value += str[pos_s++];
        }
      //}
    }
    else {
      if (format[pos_f] != str[pos_s]) {
        return false;
      }
      
      pos_f++;
      pos_s++;
    }
  }
  
  if (var){
    cout << "Var " << name << " = " << var_value << endl;
    args.push_back(pair<string,string>(name,var_value));
  }
  
  if (pos_f < (int)format.length()) return false;
  else return true;
}



struct CLI_parser {
  vector<CLI_parser_rule> rules;
  vector<string> args;
  int arg_pos;
  
  void parse(int argc,char *argv[]);
  string & get_arg();
  bool next_arg();
  void set_arg_pos(int pos);
  
};

void CLI_parser::parse(int argc,char *argv[]) {
  // Probably a good idea to clear the args vector if this is called more than once
  args.clear();
  
  for (int i = 0; i < argc; i++) {
    args.push_back(argv[i]);
  }
  
  set_arg_pos(0);
}

// Gets the current argument
string & CLI_parser::get_arg() {
  assert(arg_pos < (int)args.size());
  
  return args[arg_pos];
}

// Advances to the next argument
// Returns whether there is another argument
bool CLI_parser::next_arg() {
  return ++arg_pos < (int)args.size();
}

// Sets the position of the current argument
void CLI_parser::set_arg_pos(int pos) {
  if (pos < (int)args.size())
    arg_pos = pos;
  else
    arg_pos = args.size();
}


int main(int argc,char *argv[]){
  CLI_parser parser;
  
  CLI_parser_rule("--[flag]=[value");
  
  parser.parse(argc,argv);
  
  do{
    cout << parser.get_arg() << endl;
  } while(parser.next_arg());
}























