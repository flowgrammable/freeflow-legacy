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

#include <freeflow/sys/reactor.hpp>

using namespace freeflow;

// The purpose of this test is simply to ensure that a custom event 
// loop can be constructed using the reactor. This test just runs
// the reactor 10 times for 250ms each time. 
//
// TODO: Actually handle some events!

int main() {
  Reactor r;

  for (int i = 0; i < 10; ++i) {
    std::cout << "* running\n";
    r.run(250_ms);
  }
}
