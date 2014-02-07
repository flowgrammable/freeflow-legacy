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

#ifndef FREEFLOW_KQUEUE_HPP
#define FREEFLOW_KQUEUE_HPP

#include <unistd.h>
#include <sys/event.h>
#include <sys/types.h>

#include <map>
#include <vector>

#include "freeflow/sys/error.hpp"
#include "freeflow/sys/time.hpp"

namespace freeflow {

struct KQueue
{
  KQueue();
  ~KQueue();

  std::map<int,struct kevent> in_events;
  std::vector<struct kevent> out_events;
  int queue;
};

void add_reader(KQueue& kq, int fd);
void add_writer(KQueue& kq, int fd);
void del_reader(KQueue& kq, int fd);
void del_writer(KQueue& kq, int fd);

int kevent(KQueue& kq, const MicroTime& mt);

} // namespace freeflow

#include "kqueue.ipp"

#endif
