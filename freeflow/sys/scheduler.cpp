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
Scheduler::add(Task* t) {
  tasks.insert({t->fd(), t});
  if(t->type & Task::READABLE)
    select.add_reader(t->fd());
  if(t->type & Task::WRITABLE)
    select.add_writer(t->fd());
  t->init(now());
}

void
Scheduler::remove(Task* t) {
  tasks.erase(t->fd());
  if(t->type & Task::READABLE)
    select.remove_reader(t->fd());
  if(t->type & Task::WRITABLE)
    select.remove_writer(t->fd());
  t->finish(now());
}

void
process_task(Scheduler& s, Task* t, const Time_point& p) {
  if(s.select.is_readable(t->fd()))
    t->read(p);
  if(s.select.is_writable(t->fd()))
    t->write(p);
  t->time(p);
}

void
execute_round(Scheduler& s) {
  // Execute the select
  s.select(s.timeout);

  // Build a priority queue of tasks.
  std::vector<Task*> q(s.tasks.size());
  for (auto x : s.tasks)
    q.push_back(x.second);
  std::make_heap(q.begin(), q.end(), Less);
  
  // Execute the run queue 
  while(not q.empty()) {
    Task* task = q.front();
    std::pop_heap(q.begin(), q.end());
    q.pop_back();
    process_task(s, task, now());
  }
}

} // namespace freeflow
