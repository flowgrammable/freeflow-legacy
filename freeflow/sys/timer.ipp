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

namespace freeflow {

// -------------------------------------------------------------------------- //
// Timers

/// A timer is used internally by the timer queue.
struct Timer_queue::Timer {
  Timer(Handler*, int, Time_point);

  Handler* handler;    // The registered handler
  int id;              // The timer id
  Time_point time; // When the timer will trigger
};

inline
Timer_queue::Timer::Timer(Handler* h, int id, Time_point t) 
  : handler(h), id(id), time(t) { }

// Defines less than for timers.
struct Timer_queue::Timer_less {
  bool 
  operator()(const Timer& a, const Timer& b) const {
    return a.time < b.time;
  }
};

// -------------------------------------------------------------------------- //
// Timer queue

inline 
Timer_queue::Timer_queue() 
  : heap_(), trigger_()
{
  heap_.reserve(64);
  trigger_.reserve(32);
}

/// Schedule a timer to trigger in some amount of time from now. Note that
/// the handler/id pair not already be in the queue.
///
/// \todo Implement a predicate that asserts the precondition.
inline void 
Timer_queue::schedule(Handler* h, int id, Microseconds ms) { 
  push(h, id, now() + ms); 
}

/// Removes the timer associated with the handler that has the given id.
///
/// \todo This is not efficient. It should be more efficient if
/// the heap allowed updating (i.e., a mutable heap). A simpler idea
/// is to "deactivate" the timer by setting a flag indicating that it
/// should not trigger. This works well unless the intent is to
/// reschedule a timer with a known id.
inline void
Timer_queue::cancel(Handler* h, int id) {
  auto i = std::find_if(heap_.begin(), heap_.end(), [h, id](const Timer& t) {
    return t.handler == h and t.id == id;
  });
  if (i != heap_.end()) {
    heap_.erase(i);
    std::sort_heap(heap_.begin(), heap_.end(), Timer_less{});
  }
}

/// Removes all timers associated with the handler.
inline void
Timer_queue::cancel(Handler* h) {
  auto i = std::remove_if(heap_.begin(), heap_.end(), [h](const Timer& t) {
    return t.handler == h;
  });
  if (i != heap_.end()) {
    heap_.erase(i, heap_.end());
    std::sort_heap(heap_.begin(), heap_.end(), Timer_less{});
  }
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
Timer_queue::push(Handler* h, int id, Time_point t) {
  heap_.emplace_back(h, id, t);
  std::push_heap(heap_.begin(), heap_.end(), Timer_less{});
}

inline void
Timer_queue::pop() {
  std::pop_heap(heap_.begin(), heap_.end(), Timer_less{});
  heap_.pop_back();
}

} // namespace freeflow

