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

inline
Timer::Timer(Event_handler* h, Time_point t, Timer_id id) 
  : handler(h), time(t), id(id) { }

// Defines default ordering for timers based on their time point
struct Timer::Less {
  bool 
  operator()(const Timer& a, const Timer& b) const {
    return a.time < b.time;
  }
};


// -------------------------------------------------------------------------- //
// Timer queue

inline 
Timer_queue::Timer_queue() : heap_()
{
  heap_.reserve(64);
}

/// Schedule a timer to trigger in some amount of time from now. Note that
/// the handler/id pair not already be in the queue.
///
/// \todo Implement a predicate that asserts the precondition.
inline void 
Timer_queue::schedule(Event_handler* h, int id, Microseconds ms) { 
  push(h, id, now() + ms); 
}

/// Reschedule an existing timer to trigger in some amount of time from
/// now. Note that the handler/id pair must be in the queue.
inline void 
Timer_queue::reschedule(Event_handler* h, int id, Microseconds ms) { 
  cancel(h, id);
  schedule(h, id, ms);
}

/// Removes the timer associated with the handler that has the given id.
///
/// \todo This is not efficient. It should be more efficient if
/// the heap allowed updating (i.e., a mutable heap). A simpler idea
/// is to "deactivate" the timer by setting a flag indicating that it
/// should not trigger. This works well unless the intent is to
/// reschedule a timer with a known id.
inline void
Timer_queue::cancel(Event_handler* h, int id) {
  auto i = std::find_if(heap_.begin(), heap_.end(), [h, id](const Timer& t) {
    return t.handler == h and t.id == id;
  });
  if (i != heap_.end()) {
    heap_.erase(i);
    std::make_heap(heap_.begin(), heap_.end(), Timer::Less{});
  }
}

/// Removes all timers associated with the handler.
inline void
Timer_queue::cancel(Event_handler* h) {
  auto i = std::remove_if(heap_.begin(), heap_.end(), [h](const Timer& t) {
    return t.handler == h;
  });
  if (i != heap_.end()) {
    heap_.erase(i, heap_.end());
    std::make_heap(heap_.begin(), heap_.end(), Timer::Less{});
  }
}

/// Remove the top timer from the heap. Behavior is undefined if heap has
/// expired (relative to some time point) timers.
inline Timer
Timer_queue::expire() {
  Timer t = top();
  pop();
  return t;
}

/// Returns true if the heap contains no timers.
inline bool
Timer_queue::empty() const { return heap_.empty(); }

/// Returns true if there is at least one timer that has expired before
/// the given time point.
inline bool
Timer_queue::expired(Time_point t) const { 
  return not empty() and top().time < t; 
}

/// Returns the timer at the top of the non-empty heap.
inline Timer&
Timer_queue::top() { return heap_.front(); }

inline const Timer&
Timer_queue::top() const { return heap_.front(); }

inline void
Timer_queue::push(Event_handler* h, int id, Time_point t) {
  heap_.emplace_back(h, t, id);
  std::push_heap(heap_.begin(), heap_.end(), Timer::Less{});
}

inline void
Timer_queue::pop() {
  std::pop_heap(heap_.begin(), heap_.end(), Timer::Less{});
  heap_.pop_back();
}

} // namespace freeflow

