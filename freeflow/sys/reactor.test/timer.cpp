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

/// This is a simple service hanlder that schedules a time event
/// every .25 seconds and prints a message. No other events are 
/// procesed.
struct Printer : Resource_handler {
  Printer(Reactor& r)
    : Resource_handler(r, TIME_EVENTS, 0) { }

  bool on_open() {
    std::cout << "* open\n";
    reactor().schedule_timer(this, 0, 250_ms);
    return true;
  }

  bool on_time(int t) {
    std::cout << "* time event\n";
    ++count;
    if (count == 10)
      reactor().stop();
    else
      reactor().schedule_timer(this, 0, 250_ms);
    return true;
  }

  int count = 0;
};


int main() {
  Reactor r;
  Printer p(r);
  r.add_handler(&p);
  r.run();
}
