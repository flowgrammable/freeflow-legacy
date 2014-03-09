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

#ifndef FREEFLOW_OFPV1_0_HPP
#define FREEFLOW_OFPV1_0_HPP

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/ofp.hpp>

#include "port.hpp"

namespace freeflow {
namespace ofp {
namespace v1_0 {

enum Message_type : Uint16 {
  HELLO                = 0, 
  ERROR                = 1, 
  ECHO_REQ             = 2, 
  ECHO_RES             = 3, 
  VENDOR               = 4,
  FEATURE_REQ          = 5, 
  FEATURE_RES          = 6, 
  GET_CONFIG_REQ       = 7, 
  GET_CONFIG_RES       = 8,
  SET_CONFIG           = 9,
  PACKET_IN            = 10, 
  FLOW_REMOVED         = 11, 
  PORT_STATUS          = 12, 
  PACKET_OUT           = 13,
  FLOW_MOD             = 14, 
  PORT_MOD             = 15,
  STATS_REQ            = 16, 
  STATS_RES            = 17, 
  BARRIER_REQ          = 18, 
  BARRIER_RES          = 19,
  QUEUE_GET_CONFIG_REQ = 20, 
  QUEUE_GET_CONFIG_RES = 21
};

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#include "message.ipp"

#endif
