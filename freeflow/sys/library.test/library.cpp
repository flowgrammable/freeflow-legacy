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

#include <freeflow/sys/library.hpp>
#include <freeflow/sdn/application.hpp>
#include <apps/noflow/noflow.hpp>

using namespace freeflow;


int main() {
  
  Library l1("./apps/noflow/libnoflow.so");
  
// test loading a second of the same application
  Library L2("./apps/noflow/libnoflow.so"); 
  
// get the factory function from the shared library
  create_t* l1_factory_maker = (create_t*)l1.symbol("factory");
  
// create the factory object
  Application_factory* l1_factory = l1_factory_maker();
  
// create the application object
  Application* l1_app = l1_factory->make();

  l1_app->test();
}

