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

#include <freeflow/proto/ofp/v1.0/protocol.hpp>

namespace freeflow {
namespace ofp {
namespace v1_0 {

void 
Handler::to_feature() {
  Empty p;
  Header h(FEATURE_REQUEST, bytes(p), 0);
  put_message(h, p);
}

void 
Handler::wait_feature() {
  Header h;
  peek_header(h);

  // FIXME: Error handling.
  if (h.type != FEATURE_REPLY) {
    return;
  }
  
  // Get the feature... and do what?
  Feature m;
  get_payload(h, m);

  proto_.to_established();
}

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

