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

/// This handler the results of signals.
struct Signal_handler : Resource_handler {
  Signal_handler(Reactor& r)
    : Resource_handler(r, SIGNAL_EVENTS, 0) { }

  bool on_open() {
    std::cout << "* starting " << ::getpid() << '\n';
    return true;
  }

  // TODO: Emit a meaningful string
  bool on_signal(int s) {
    std::cout << "* signal: " << ::strsignal(s) << '\n';
    return true;
  }
};


int main() {
  Reactor r;
  Signal_handler sh(r);
  r.add_handler(&sh);
  r.run();
}
