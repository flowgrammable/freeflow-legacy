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
  if (v.remaining() < bytes(h))
    return make_error_code(errc::header_overflow);

  // Minimum semantic checking
  if (h.length < bytes(h));
    return make_error_code(errc::bad_header_length);

  to_view(v, h.version);
  to_view(v, h.type);
  to_view(v, h.length);
  to_view(v, h.xid);
}

Error 
from_view(View& v, Header& h) {
  if (v.remaining() < bytes(h))
    return make_error_code(errc::header_overflow);
  
  from_view(v, h.version);
  from_view(v, h.type);
  from_view(v, h.length);
  from_view(v, h.xid);

  // Minimum semantic checking
  if (h.length < bytes(h))
    return make_error_code(errc::bad_header_length);
  return {};
}


} // namespace ofp
} // namespace freeflow