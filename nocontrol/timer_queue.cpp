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

#include "timer_queue.hpp"

using namespace ff;

namespace nocontrol {

/// Check to see if which timers have expired, if any. Notify registered
/// handlers of the triggered timer.
void
Timer_queue::dispatch(Reactor& r) {
  // Move triggered timers to the a different queue.
  Time_point t = now();
  while (not empty() and top().time < t) {
    trigger_.push_back(top());
    pop();
  }

  // Notify timers in that queue and clear it.
  for (const Timer& t : trigger_)
    t.handler->on_time(r, t.id);
  trigger_.clear();
}

} // namespace nocontrol