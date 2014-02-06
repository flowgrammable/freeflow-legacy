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

namespace freeflow {

inline void
add_job(Scheduler& s, Job* j)
{
  s.job_queue.push(j);
}

inline void
Scheduler::exec(Job* j, int tick, int max_tick)
{
  
}

inline void
Scheduler::run()
{
  while(jobs.length() > 0) {
    std::priority_queue<Job*> run_queue = jobs.mk_queue();
    r = select(sel, 0);
    while(instance.length() > 0) {
      x = instance.top();
      x
    }
  }
}

} // namespace freeflow
