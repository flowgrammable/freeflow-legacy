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
#include <cassert>

#include <freeflow/sys/library.hpp>
#include <freeflow/sdn/application.hpp>

using namespace std;
using namespace freeflow;


int main() {
  // Assert that l1 and l2 are the same library.
  Library l1("flog_noflow.app");
  Library l2("flog_noflow.app"); 
  assert(l1 == l2);

  Library l3("flog_template.app");
  assert(l1 != l3);

  // TODO: Write unloading tests

}

