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

#ifndef FREEFLOW_TIMER_HPP
#define FREEFLOW_TIMER_HPP

#include <vector>

#include <freeflow/sys/time.hpp>

namespace freeflow {

class Event_handler;
class Reactor;

/// A Timer_id uniquely identifies a timer in a queue.
using Timer_id = int;

/// The Timer structure represnts an entry in a timre queue. The
/// structure binds together information about an event handler, the
/// time point at which the timer triggers, and the timer's identifier.
struct Timer {
  struct Less;

  Timer(Event_handler*, Time_point, Timer_id);

  Event_handler* handler; // The registered handler
  Time_point     time;    // When the timer will trigger
  Timer_id       id;      // The timer id
};

/// A Timer_list is a sequence of timers.
using Timer_list = std::vector<Timer>;

/// The Timer_queue is allows handlers to be scheduled for timeout 
/// events. The timer has microsecond granularity.
///
/// A timer is scheduled with a handler and an identifier. The identifier
/// is an integer value that designates the purpose of the timer for the
/// application. This value is passed to the handler on notification.
///
/// \todo Consider returning timer ids when scheduling a timer. This allows 
/// handlers to cancel specific timers and not all timers. 
///
/// \todo Allow repeating timers to be scheduled.
class Timer_queue {
public:
  Timer_queue();

  // Scheduling and extraction
  void schedule(Event_handler*, int, Microseconds);
  void reschedule(Event_handler*, int, Microseconds);
  void cancel(Event_handler*, int);
  void cancel(Event_handler*);
  Timer expire();

  // Observers
  bool empty() const;
  bool expired(Time_point) const;

private:
  Timer&       top();
  const Timer& top() const;

  void push(Event_handler*, int, Time_point);
  void pop();

private:
  Timer_list heap_;    // The actual queue
};

} // namespace freeflow

#include <freeflow/sys/timer.ipp>

#endif
