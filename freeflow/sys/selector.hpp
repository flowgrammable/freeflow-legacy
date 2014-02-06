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

#include <sys/select.h>
#include <set>

#include "freeflow/sys/error.hpp"

namespace freeflow {

struct Selector
{
  Selector();

  fd_set readset;
  fd_set writeset;
  std::set<int> read_fds;
  std::set<int> write_fds;
};

int max(const Selector& s);

void add_reader(Selector& s, int fd);
void add_writer(Selector& s, int fd);
void del_reader(Selector& s, int fd);
void del_writer(Selector& s, int fd);

bool is_readable(const Selector& s, int fd);
bool is_writable(const Selector& s, int fd);

int select(timeval* tv);

} // namespace freeflow

#include "selector.ipp"

#endif
