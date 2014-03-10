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

#include "port.hpp"

namespace freeflow {
namespace ofp {
namespace v1_0 {

using ofp::to_view;
using ofp::from_view;

Errc
to_view(View& v, const Port& p) {
  to_view(v, p.port_id);
  to_view(v, p.hw_addr);
  to_view(v, p.name);
  to_view(v, p.config);
  to_view(v, p.state);
  to_view(v, p.current);
  to_view(v, p.advertised);
  to_view(v, p.supported);
  to_view(v, p.peer);
  return {};  
}


Errc
from_view(View& v, Port& p) {
  from_view(v, p.port_id);
  from_view(v, p.hw_addr);
  from_view(v, p.name);
  from_view(v, p.config);
  from_view(v, p.state);
  from_view(v, p.current);
  from_view(v, p.advertised);
  from_view(v, p.supported);
  from_view(v, p.peer);
  return {};  
}

} // namespace v1_0
} // namespace ofp
} // namespace freeflow
