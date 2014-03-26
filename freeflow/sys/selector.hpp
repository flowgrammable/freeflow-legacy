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

#include <unistd.h>
#include <cassert>

#include <set>

#include "freeflow/sys/error.hpp"
#include "freeflow/sys/time.hpp"
#include "freeflow/sys/resource.hpp"

namespace freeflow {

/// The Select set maintains resource sets for common events. 
/// This is primarily used by the select function to represent
/// what resources are waiting in select and and which have
/// actions pending.
struct Select_set {
  Resource_set read;
  Resource_set write;
  Resource_set error;
};

/// The selector class provides a simple wrapper over the POSIX pselect
/// function. It maintains the sets of file descriptors used to indicate
/// the resources that are available for reading and writing.
class Selector {
public:
  Selector(int n, Select_set& ss);

  // Select
  int operator()();

private:
  int     max_;
  fd_set* read_;
  fd_set* write_;
  fd_set* error_;
};

} // namespace freeflow

#include "selector.ipp"

#endif
