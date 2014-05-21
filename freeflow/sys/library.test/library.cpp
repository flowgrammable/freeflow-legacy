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

using namespace freeflow;


int main() {
  
  Library l1("./apps/noflow/libnoflow.so");
  
  // test loading a second of the same application
  Library l2("./apps/noflow/libnoflow.so"); 
  
  // FIXME: assert that l1 and l2 are the same library.
  // assert(l1 == l2);
  
  
  // get the factory function from the shared library
  Application_factory_fn fn = l1.function<Application_factory_fn>("factory");
  
  // create the factory object
  Application_factory* fact = fn();
  
  // create the application object
  Application* app = fact->make();

  app->test();
}

