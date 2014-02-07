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

inline
Task::Task(int t, int p) :
  priority(p), type(t)
{ }

inline void
Task::set_readable()
{
  type |= READABLE;
}

inline void
Task::set_writable()
{
  type |= WRITABLE;
}

inline void
Task::clr_readable()
{
  type &= ~READABLE;
}

inline void
Task::clr_writable()
{
  type &= ~WRITABLE;
}

inline bool
Task::readable() const
{
  return type & READABLE;
}

inline bool
Task::writable() const
{
  return type & WRITABLE;
}


inline bool
Less(const Task* lhs, const Task* rhs)
{
  return rhs->priority < lhs->priority;
}

inline void
clr_tasks(Scheduler& s)
{
  s.tasks.clear();
}

inline void
run(Scheduler& s)
{
  while(s.tasks.size() > 0)
    execute_round(s);
}

} // namespace freeflow

