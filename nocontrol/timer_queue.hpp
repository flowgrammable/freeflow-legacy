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

#ifndef NOCONTROL_TIMER_QUEUE_HPP
#define NOCONTROL_TIMER_QUEUE_HPP

#include <vector>

#include <freeflow/sys/time.hpp>

#include <nocontrol/config.hpp>
#include <nocontrol/handler.hpp>

namespace nocontrol {


/// The Timer_queue is allows handlers to be scheduled for timeout 
/// events. The timer has microsecond granularity
///
/// \todo This should be freeflow.
///
/// \todo Return timer ids when scheduling a timer. This allows handlers
/// to cancel specific timers and not all timers.
///
/// \todo Allow repeating timers to be scheduled.
class Timer_queue {
  struct Timer;
  struct Timer_less;
public:
  // Scheduling
  void schedule(Handler*, ff::Microseconds);
  void cancel(Handler*);

  // Dispatch.
  void dispatch();

  // Observers
  bool empty() const;

private:
  Timer&       top();
  const Timer& top() const;

  void push(Handler*, ff::Time_point);
  void pop();

private:
  std::vector<Timer> heap_;
};

} // namespace nocontrol

#include "timer_queue.ipp"

#endif
