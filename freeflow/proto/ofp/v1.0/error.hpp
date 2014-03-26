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

// Note that this class is named "Errc" (for error code) in this 
// namespace to avoid collisions with the Error message.
//
// TODO: Renumber error codes in some meaningful way.
struct Errc : ofp::Error {
  static constexpr Code BAD_VERSION = 100;

  /// The message type is unsupported
  static constexpr Code BAD_MESSAGE_TYPE = 200;
  static constexpr Code BAD_ACTION_TYPE = 300;
  static constexpr Code BAD_STATS_TYPE = 400;
  static constexpr Code BAD_PROPERTY_TYPE = 500;

  static constexpr Code HELLO_OVERFLOW   = 201;
  static constexpr Code ERROR_OVERFLOW   = 202;
  static constexpr Code ECHO_OVERFLOW    = 203;
  static constexpr Code VENDOR_OVERFLOW  = 204;
  static constexpr Code FEATURE_OVERFLOW = 205;
  static constexpr Code CONFIG_OVERFLOW  = 206;
  static constexpr Code PACKET_IN_OVERFLOW     = 207;
  static constexpr Code FLOW_REMOVED_OVERFLOW  = 208;
  static constexpr Code PORT_STATUS_OVERFLOW   = 209;
  static constexpr Code PACKET_OUT_OVERFLOW    = 210;
  static constexpr Code FLOW_MOD_OVERFLOW      = 211;
  static constexpr Code PORT_MOD_OVERFLOW      = 212;
  static constexpr Code STATS_REQUEST_OVERFLOW = 213;
  static constexpr Code STATS_REPLY_OVERFLOW   = 214;
  static constexpr Code QUEUE_CONFIG_REQUEST_OVERFLOW = 215;
  static constexpr Code QUEUE_CONFIG_REPLY_OVERFLOW   = 216;
  static constexpr Code MESSAGE_OVERFLOW = 220;

  static constexpr Code ACTION_OVERFLOW = 250;
  static constexpr Code BAD_ACTION_LENGTH = 301;

  static constexpr Code FLOW_STATS_OVERFLOW = 401;
  static constexpr Code QUEUE_STATS_OVERFLOW = 402;
  static constexpr Code VENDOR_STATS_OVERFLOW = 403;
  static constexpr Code BAD_FLOW_STATS_LENGTH = 410;
  static constexpr Code BAD_QUEUE_STATS_LENGTH = 411;

  static constexpr Code PROPERTY_OVERFLOW = 550;
  static constexpr Code BAD_PROPERTY_LENGTH = 501;
  static constexpr Code BAD_QUEUE_LENGTH = 501;


  using ofp::Error::Error;
};

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#endif
