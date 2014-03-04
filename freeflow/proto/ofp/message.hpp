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

#ifndef FREEFLOW_OFP_MESSAGE_HPP
#define FREEFLOW_OFP_MESSAGE_HPP

#include <freeflow/sys/meta.hpp>
#include <freeflow/sys/buffer.hpp>

namespace freeflow {

bool to_buffer(Buffer_view& v, uint8_t n);
bool to_buffer(Buffer_view& v, uint16_t n);
bool to_buffer(Buffer_view& v, uint32_t n);
bool to_buffer(Buffer_view& v, uint64_t n);

template<typename T, typename = Requires<Enum<T>()>>
  bool to_buffer(Buffer_view& v, T value);


/// The OpenFlow header is found at the front of every OpenFlow message.
struct Header {
  uint8_t version;
  uint8_t type;
  uint16_t lenght;
  uint32_t xid;
};



} // freeflow

#include <freeflow/proto/ofp/message.ipp>

#endif
