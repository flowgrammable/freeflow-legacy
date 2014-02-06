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

#ifndef FREEFLOW_SELECTOR_HPP
#define FREEFLOW_SELECTOR_HPP

#include <queue>

#include "freeflow/sys/error.hpp"

namespace freeflow {

struct Job
{
  virtual void init() = 0;
  virtual void fini() = 0;

  virtual void read() = 0;
  virtual void write() = 0;

  virtual void time() = 0;
  virtual int fd() const = 0;

  int priority;
  int ticks;
  int max_ticks;
};

bool operator<(const Job& lhs, const Job& rhs) const;

struct Scheduler
{
  std::priority_queue<Job*> job_queue;  
};

void add_job(Scheduler& sched, Job* sel);
void del_job(Scheduler& sched, Job* sel);
void run(Scheduler& s);

} // namespace freeflow

#include "scheduler.ipp"

#endif
