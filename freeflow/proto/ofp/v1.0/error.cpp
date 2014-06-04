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

#include "error.hpp"

namespace freeflow {
namespace ofp {
namespace v1_0 {

namespace {
struct Errors : Error_category {
  const char* name() const noexcept override;
  std::string message(int ec) const override;
};

const char* 
Errors::name() const noexcept { return "openflow v1.0"; }

std::string 
Errors::message(int ec) const {
  switch (static_cast<errc>(ec)) {
  case errc::bad_version: return "bad version";
  case errc::bad_header_length: return "bad heeader length";
  case errc::bad_message_type: return "bad message type";
  case errc::bad_action_type: return "bad action type";
  case errc::bad_stats_type: return "bad stats type";
  case errc::bad_property_type: return "bad property type";

  case errc::hello_overflow: return "hello overflow";
  case errc::error_overflow: return "error overflow";
  case errc::echo_overflow: return "echo overflow";
  case errc::vendor_overflow: return "vendor overflow";
  case errc::feature_overflow: return "feature overflow";
  case errc::config_overflow: return "config overflow";
  case errc::packet_in_overflow : return "packet-in overflow";
  case errc::flow_removed_overflow: return "flow-removed overflow";
  case errc::port_status_overflow: return "port-status overflow";
  case errc::packet_out_overflow: return "packet-out overflow";
  case errc::flow_mod_overflow: return "flow-mod overflow";
  case errc::port_mod_overflow: return "port-mod overflow";
  case errc::stats_request_overflow: return "stats-request overflow";
  case errc::stats_reply_overflow: return "stats-reply overflow";
  case errc::queue_config_request_overflow: return "queue-config-request overflow";
  case errc::queue_config_reply_overflow: return "queue-config-reply overflow";
  case errc::message_overflow: return "message overflow";

  case errc::action_overflow: return "action overflow";
  case errc::bad_action_length: return "bad action length";

  case errc::flow_stats_overflow: return "flow stats overflow";
  case errc::queue_stats_overflow: return "queue stats overflow";
  case errc::vendor_stats_overflow: return "vendor stats overflow";
  case errc::bad_flow_stats_length: return "bad flow stats length";
  case errc::bad_queue_stats_length: return "bad queue stats length";

  case errc::property_overflow: return "property overflow";
  case errc::bad_property_length: return "bad property length";
  case errc::bad_queue_length: return "bad queue length";
  }
  assert(false); // Unreachable
  return "";
}
} // namesapce

const Error_category&
error_category() {
  static Errors errs_;
  return errs_;
}

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

