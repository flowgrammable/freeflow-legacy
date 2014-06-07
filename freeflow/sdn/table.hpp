// Copyright (c) 2013-2014 Flowgrammable, LLC.
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

#ifndef FREEFLOW_TABLE_HPP
#define FREEFLOW_TABLE_HPP

#include <vector>

#include <freeflow/sdn/match.hpp>
#include <freeflow/sdn/action.hpp>

namespace freeflow {

struct Flow;

struct Flow_table {
  std::vector<Flow> flows;
};

struct Flow{
  Match match;
  Action action;
  // Instruction instruction; ?
};

} // namespace freeflow

#include <freeflow/sdn/table.ipp>

#endif