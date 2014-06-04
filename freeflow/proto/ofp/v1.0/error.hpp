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

#ifndef FREEFLOW_OFPV1_0_ERROR_HPP
#define FREEFLOW_OFPV1_0_ERROR_HPP

#include <freeflow/sys/error.hpp>

namespace freeflow {
namespace ofp {
namespace v1_0 {

enum class errc {
  bad_version,

  bad_header_length,
  
  bad_message_type,
  message_overflow,
  
  hello_overflow,
  error_overflow,
  echo_overflow,
  vendor_overflow,
  feature_overflow,
  config_overflow,
  packet_in_overflow ,
  flow_removed_overflow,
  port_status_overflow,
  packet_out_overflow,
  flow_mod_overflow,
  port_mod_overflow,
  stats_request_overflow,
  stats_reply_overflow,
  queue_config_request_overflow,
  queue_config_reply_overflow,

  bad_action_type,
  bad_action_length,
  action_overflow,

  bad_stats_type,
  flow_stats_overflow,
  queue_stats_overflow,
  vendor_stats_overflow,
  bad_flow_stats_length,
  bad_queue_stats_length,

  bad_property_type,
  property_overflow,
  bad_property_length,
  bad_queue_length,

  // Semantic errors
  // TODO: Write out the other errors in Error_message
  connection_refused,     // HF_EPERM
  incompatible_version,   // HF_INCOMPATIBLE
};

const Error_category& error_category();
Error_code make_error_code(errc);

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#include <freeflow/proto/ofp/v1.0/error.ipp>

#endif
