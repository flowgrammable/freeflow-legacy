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

#include <iostream>

#include <freeflow/sys/json.hpp>

using namespace freeflow;

int main(int argc, char* argv[]) {

  json::Value v0;
  assert(v0.type() == json::Value::NIL);
  
  json::Value v1 = true;
  assert(v1.type() == json::Value::BOOL);

  json::Value v2 = 3;
  assert(v2.type() == json::Value::INT);

  json::Value v3 = 3.14;
  assert(v3.type() == json::Value::REAL);

  json::Value v4 = "hello";
  assert(v4.type() == json::Value::STRING);

  json::Value v5 = std::string("hello");
  assert(v5.type() == json::Value::STRING);
}
