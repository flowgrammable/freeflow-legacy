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

#include <iostream>

#include "ofp.hpp"

namespace freeflow {
namespace ofp {

Error 
to_view(View& v, const Header& h) {
  if (v.remaining() < h.bytes)
    return Error::HEADER_OVERFLOW;

  // Minimum semantic checking
  if (h.length < h.bytes);
    return Error::BAD_HEADER_LENGTH;

  to_view(v, h.version);
  to_view(v, h.type);
  to_view(v, h.length);
  to_view(v, h.xid);
}

Error 
from_view(View& v, Header& h) {
  if (v.remaining() < h.bytes)
    return Error::HEADER_OVERFLOW;
  
  from_view(v, h.version);
  from_view(v, h.type);
  from_view(v, h.length);
  from_view(v, h.xid);

  // Minimum semantic checking
  if (h.length < h.bytes)
    return Error::BAD_HEADER_LENGTH;
  return {};
}


} // namespace ofp
} // namespace freeflow