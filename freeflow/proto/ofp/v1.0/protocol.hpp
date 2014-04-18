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

#ifndef FREEFLOW_OFPV1_0_PROTOCOL_HPP
#define FREEFLOW_OFPV1_0_PROTOCOL_HPP

#include <freeflow/proto/ofp/v1.0/message.hpp>

namespace freeflow {
namespace ofp {
namespace v1_0 {

/// The Handler class is responsible for the transacting the version
/// specific aspects of the OpenFlow protocol.
struct Handler : ofp::Protocol_handler {
  Handler(Protocol& p) 
    : proto_(p) 
  { }

  // Read/write helper functions
  template<typename H, typename P>
    void put_message(const H& h, const P& p) { proto_.put_message(h, p); }

  template<typename H>
    void peek_header(H& h) { proto_.peek_header(h); }

  template<typename H, typename P>
    void get_payload(const H& h, P& p) { proto_.get_payload(h, p); }

  // Events and messages
  void to_feature();
  void wait_feature();


  Protocol& proto_;
};

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#endif

