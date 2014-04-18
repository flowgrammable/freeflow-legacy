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

class Handler;
class Reactor;

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
  struct Timer;
  struct Timer_less;
  using Timer_list = std::vector<Timer>;
public:
  Timer_queue();

  // Scheduling
  void schedule(Handler*, int, Microseconds);
  void cancel(Handler*, int);
  void cancel(Handler*);

  // Dispatch.
  void dispatch(Reactor&);

  // Observers
  bool empty() const;

private:
  Timer&       top();
  const Timer& top() const;

  void push(Handler*, int, Time_point);
  void pop();

private:
  Timer_list heap_;    // The actual queue
  Timer_list trigger_; // Triggered timers
};

} // namespace freeflow

#include <freeflow/sys/timer.ipp>

#endif
