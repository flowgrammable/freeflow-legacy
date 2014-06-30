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

#ifndef FREEFLOW_QUEUE_HPP
#define FREEFLOW_QUEUE_HPP

#include <vector>
#include <map>

#include <freeflow/sys/data.hpp>

namespace freeflow {

struct Queue {
  Uint32 queue_id;
  bool   min_rate; // From QueueGetConfigRes.queues[].property
  int    rate;     // From QueueGetConfigRes.queues[].rate

};

struct Queues {
  bool pause_asym;   // From FeatureRes.ports[].supported
  bool queue_stats;  // From FeatureRes.capabilities
  std::vector<Queue> queues;
};

} // namespace freeflow

#include <freeflow/sdn/queue.ipp>

#endif