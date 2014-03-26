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

#include <freeflow/proto/ofp/v1.0/message.hpp>

#include "protocol.hpp"

namespace freeflow {
namespace ofp {

void 
Protocol::open() {
  // This is gross.
  v1_0::Message m {
    /* Header */  {v1_0::VERSION, v1_0::HELLO, 8, 0},
    /* Payload */ { }
  };
  write.put_message(m);
}

void 
Protocol::close() {

}

void
Protocol::message() {

}

void
Protocol::time() {

}


} // namespace ofp
} // namespace freeflow