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
add_job(Scheduler& s, Job* j)
{
  s.jobs.insert(std::pair<int,Job*>(j->fd(), j));
  if(j->type & Job::READABLE)
    add_reader(s.sel, j->fd());
  if(j->type & Job::WRITABLE)
    add_writer(s.sel, j->fd());
  j->init();
}

void
del_job(Scheduler& s, Job* j)
{
  s.jobs.erase(j->fd());
  if(j->type & Job::READABLE)
    del_reader(s.sel, j->fd());
  if(j->type & Job::WRITABLE)
    del_writer(s.sel, j->fd());
  j->fini();
}

void
process_job(Scheduler& s, Job* j)
{
  if(is_readable(s.sel, j->fd())) {
    j->read();
  }
  if(is_writable(s.sel, j->fd())) {
    j->write();
  }
  j->time();
}

void
execute_round(Scheduler& s)
{
  // Build a run queue
  std::vector<Job*> run_queue;
  std::make_heap (run_queue.begin(), run_queue.end());    // is this necessary?
  for(auto job : s.jobs) {
    run_queue.push_back(job.second);
    std::push_heap(run_queue.begin(), run_queue.end(), Less);
  }
  
  // Execute the select
  auto result = select(s.sel, nullptr);
  if(result == -1)
    throw Error(Error::SYSTEM_ERROR, errno);
  
  // Execute the run queue 
  while(run_queue.size() > 0) {
    Job* job = run_queue.front();
    std::pop_heap(run_queue.begin(), run_queue.end());
    run_queue.pop_back();
    process_job(job);
  }
}

} // namespace freeflow
