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

#include "match.hpp"

namespace freeflow {
namespace ofp {
namespace v1_0 {

using ofp::to_view;
using ofp::from_view;

Error
to_view(View& v, const Match& m) {
  to_view(v, m.wildcards);
  to_view(v, m.in_port);
  to_view(v, m.dl_src);
  to_view(v, m.dl_dst);
  to_view(v, m.dl_vlan);
  to_view(v, m.dl_pcp);
  to_view(v, m.dl_type);
  to_view(v, m.nw_tos);
  to_view(v, m.nw_proto);
  to_view(v, m.nw_src);
  to_view(v, m.nw_dst);
  to_view(v, m.tp_src);
  to_view(v, m.tp_dst);
  return {};  
}

Error
from_view(View& v, Match& m) {
  from_view(v, m.wildcards);
  from_view(v, m.in_port);
  from_view(v, m.dl_src);
  from_view(v, m.dl_dst);
  from_view(v, m.dl_vlan);
  from_view(v, m.dl_pcp);
  from_view(v, m.dl_type);
  from_view(v, m.nw_tos);
  from_view(v, m.nw_proto);
  from_view(v, m.nw_src);
  from_view(v, m.nw_dst);
  from_view(v, m.tp_src);
  from_view(v, m.tp_dst);
  return {};  
}

} // namespace v1_0
} // namespace ofp
} // namespace freeflow
