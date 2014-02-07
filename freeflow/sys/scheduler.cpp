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

#include "scheduler.hpp"

namespace freeflow {

void
add_task(Scheduler& s, Task* t)
{
  s.tasks.insert(std::pair<int,Task*>(t->fd(), t));
  if(t->type & Task::READABLE)
    add_reader(s.sel, t->fd());
  if(t->type & Task::WRITABLE)
    add_writer(s.sel, t->fd());
  TimePoint now = std::chrono::high_resolution_clock::now();
  t->init(now);
}

void
del_task(Scheduler& s, Task* t)
{
  s.tasks.erase(t->fd());
  if(t->type & Task::READABLE)
    del_reader(s.sel, t->fd());
  if(t->type & Task::WRITABLE)
    del_writer(s.sel, t->fd());
  TimePoint now = std::chrono::high_resolution_clock::now();
  t->fini(now);
}

void
process_task(Scheduler& s, Task* t, const TimePoint& now)
{
  if(is_readable(s.sel, t->fd())) {
    t->read(now);
  }
  if(is_writable(s.sel, t->fd())) {
    t->write(now);
  }
  t->time(now);
}

void
execute_round(Scheduler& s)
{
  // Execute the select
  select(s.sel, s.timeout);
  TimePoint now = std::chrono::high_resolution_clock::now();

  // Build a run queue
  std::vector<Task*> run_queue;
  std::make_heap (run_queue.begin(), run_queue.end());    // is this necessary?
  for(auto task : s.tasks) {
    run_queue.push_back(task.second);
    std::push_heap(run_queue.begin(), run_queue.end(), Less);
  }
  
  // Execute the run queue 
  while(run_queue.size() > 0) {
    Task* task = run_queue.front();
    std::pop_heap(run_queue.begin(), run_queue.end());
    run_queue.pop_back();
    process_task(s, task, now);
  }
}

} // namespace freeflow
