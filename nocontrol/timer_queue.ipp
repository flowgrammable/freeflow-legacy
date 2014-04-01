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

#include <algorithm>

namespace nocontrol {

// Defines less than for timers.
struct Timer_less {
  bool 
  operator()(const Timer& a, const Timer& b) const {
    return a.time < b.time;
  }
};

inline
Timer::Timer(Handler* h, ff::Time_point t) 
  : handler(h), time(t) { }

// Insert the timer into the queu.
inline void 
Timer_queue::schedule(Handler* h, ff::Time_point t) {
  push(h, t);
}

inline
void 
Timer_queue::cancel(Handler*) {
  // FIXME: Not implemented.
}

/// Returns true if the heap contains no timers.
inline
bool
Timer_queue::empty() const { return heap_.empty(); }

/// Returns the timer at the top of the non-empty heap.
inline Timer&
Timer_queue::top() { return heap_.front(); }

inline const Timer&
Timer_queue::top() const { return heap_.front(); }

inline void
Timer_queue::push(Handler* h, ff::Time_point t) {
  Timer_less comp;
  heap_.emplace_back(h, t);
  std::push_heap(heap_.begin(), heap_.end(), comp);
}

inline void
Timer_queue::pop() {
  Timer_less comp;
  std::pop_heap(heap_.begin(), heap_.end(), comp);
  heap_.pop_back();
}

} // namespace nocontrol

