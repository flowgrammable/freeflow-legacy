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

#include <string>
#include <map>
#include <vector>

#include <freeflow/sdn/match.hpp>
#include <freeflow/sdn/action.hpp>

namespace freeflow {

// TODO: Determine whether this struct is necessary or not.
// struct Flow {
//   Match  match;
//   Action action;
// };

/// The Flow_table structure represents a table of flows consisting 
/// of a Match and an Action for a switch. This is stored as a map 
/// associating a Match with an Action.
///
/// todo\ Should this be set up as a vector of Flows, or is this an
/// appropriate implementation?
struct Flow_table : public std::map<Match, Action> {
// struct Flow_table : public std::vector<Flow> {
  int flowtable_id; // ID of the flow table
  std::string name; // From StatsRes.Table.name
};

/// Flow_tables represents a sequence of Flow_table objects in the 
/// form of a flowtable_id to Flow_table mapping. 
struct Flow_tables : public std::map<int, Flow_table> {
  bool table_stats; // From FeatureRes.capabilities
  bool flow_stats;  // From FeatureRes.capabilities
};

} // namespace freeflow

#include <freeflow/sdn/table.ipp>

#endif