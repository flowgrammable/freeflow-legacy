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

#ifndef FREEFLOW_SCHEDULER_HPP
#define FREEFLOW_SCHEDULER_HPP

#include <queue>
#include <map>

#include "freeflow/sys/selector.hpp"

namespace freeflow {

struct Task
{
  static constexpr int CLEAR    = 0;
  static constexpr int READABLE = 1 << 0;
  static constexpr int WRITABLE = 1 << 1;

  Task(int t = CLEAR, int p = 10);

  void set_readable();
  void set_writable();
  void clr_readable();
  void clr_writable();
  bool readable() const;
  bool writable() const;

  virtual void init() {}
  virtual void fini() {}

  virtual void read()  = 0;
  virtual void write() {}
  virtual void time()  {}

  virtual int fd() const = 0;

  int priority;
  int type;
};

bool Less(const Task* lhs, const Task* rhs);

struct Scheduler
{
  std::map<int,Task*> tasks;
  Selector sel;
};

void add_task(Scheduler& sched, Task* t);
void del_task(Scheduler& sched, Task* t);
void clr_task(Scheduler& sched);

void run(Scheduler& s);
void process_task(Scheduler& s, Task* t);
void execute_round(Scheduler& s);

} // namespace freeflow

#include "scheduler.ipp"

#endif
