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

#ifndef FREEFLOW_DATAPATH_HPP
#define FREEFLOW_DATAPATH_HPP

#include <freeflow/sdn/port.hpp>
#include <freeflow/sdn/table.hpp>
#include <freeflow/sys/buffer.hpp>

namespace freeflow {

struct Datapath {
  Uint64      datapath_id;   // From FeatureRes
  bool        ip_reassembly; // From FeatureRes.capabilities
  
  // std::string mfr_desc;      // From StatsReq.Desc.mfr_desc
  // std::string hw_desc;       // From StatsReq.Desc.hw_desc
  // std::string sw_desc;       // From StatsReq.Desc.sw_desc
  // std::string serial_num;    // From StatsReq.Desc.serial_num
  // std::string dp_desc;       // From StatsReq.Desc.dp_desc


  Ports ports;
  std::vector<Flow_table> tables;
  std::vector<Buffer> buffers; // change to Packet_buffer class
};

} // namespace freeflow

#include <freeflow/sdn/datapath.ipp>

#endif