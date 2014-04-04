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

// -------------------------------------------------------------------------- //
// Timers

/// A timer is used internally by the timer queue.
struct Timer_queue::Timer {
  Timer(Handler*, ff::Time_point);

  /// The handler to be notified when a timer triggers.
  Handler* handler;

  /// The time when the timer should trigger.
  ff::Time_point time;
};

inline
Timer_queue::Timer::Timer(Handler* h, ff::Time_point t) 
  : handler(h), time(t) { }

// Defines less than for timers.
struct Timer_queue::Timer_less {
  bool 
  operator()(const Timer& a, const Timer& b) const {
    return a.time < b.time;
  }
};

// -------------------------------------------------------------------------- //
// Timer queue

/// Schedule a timer to trigger in some amount of time from now.
inline void 
Timer_queue::schedule(Handler* h, ff::Microseconds ms) { 
  push(h, ff::now() + ms); 
}

/// Removes all timers associated with the handler.
///
/// \todo This is not efficient. It should be more efficient if
/// the heap allowed updating (i.e., a mutable heap).
inline
void 
Timer_queue::cancel(Handler* h) {
  auto i = std::remove_if(heap_.begin(), heap_.end(), [h](const Timer& t) { 
    return t.handler == h; 
  });
  heap_.erase(i, heap_.end());
  std::sort_heap(heap_.begin(), heap_.end(), Timer_less{});
}

/// Returns true if the heap contains no timers.
inline bool
Timer_queue::empty() const { return heap_.empty(); }

/// Returns the timer at the top of the non-empty heap.
inline Timer_queue::Timer&
Timer_queue::top() { return heap_.front(); }

inline const Timer_queue::Timer&
Timer_queue::top() const { return heap_.front(); }

inline void
Timer_queue::push(Handler* h, ff::Time_point t) {
  heap_.emplace_back(h, t);
  std::push_heap(heap_.begin(), heap_.end(), Timer_less{});
}

inline void
Timer_queue::pop() {
  std::pop_heap(heap_.begin(), heap_.end(), Timer_less{});
  heap_.pop_back();
}

} // namespace nocontrol

