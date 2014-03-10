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

#include "action.hpp"

namespace freeflow {
namespace ofp {
namespace v1_0 {

using ofp::to_view;
using ofp::from_view;

// To view

Errc
to_view(View& v, const Action_empty& m) { return {}; }

Errc
to_view(View& v, const Action_output& m) {
  to_view(v, m.port);
  to_view(v, m.max_len);
  return {};
}

Errc
to_view(View& v, const Action_enqueue& m) {
  to_view(v, m.port);
  pad(v, 6);
  to_view(v, m.queue);
  return {};
}

Errc
to_view(View& v, const Action_vlan_vid& m) {
  to_view(v, m.value);
  pad(v, 2);
  return {};
}

Errc
to_view(View& v, const Action_vlan_pcp& m) {
  to_view(v, m.value);
  pad(v, 3);
  return {};
}

Errc
to_view(View& v, const Action_dl_addr& m) {
  to_view(v, m.addr);
  pad(v, 6);
  return {};
}

Errc
to_view(View& v, const Action_nw_addr& m) {
  to_view(v, m.addr);
  return {};
}

Errc
to_view(View& v, const Action_nw_tos& m) {
  to_view(v, m.value);
  pad(v, 3);
  return {};
}

Errc
to_view(View& v, const Action_tp_port& m) {
  to_view(v, m.port);
  pad(v, 2);
  return {};
}

Errc
to_view(View& v, const Action_vendor& m) {
  to_view(v, m.vendor);
  return {};
}

// From view

Errc
from_view(View& v, Action_empty& m) { return {}; }

Errc
from_view(View& v, Action_output& m) {
  from_view(v, m.port);
  from_view(v, m.max_len);
  return {};
}

Errc
from_view(View& v, Action_enqueue& m) {
  from_view(v, m.port);
  pad(v, 6);
  from_view(v, m.queue);
  return {};
}

Errc
from_view(View& v, Action_vlan_vid& m) {
  from_view(v, m.value);
  pad(v, 2);
  return {};
}

Errc
from_view(View& v, Action_vlan_pcp& m) {
  from_view(v, m.value);
  pad(v, 3);
  return {};
}

Errc
from_view(View& v, Action_dl_addr& m) {
  from_view(v, m.addr);
  pad(v, 6);
  return {};
}

Errc
from_view(View& v, Action_nw_addr& m) {
  from_view(v, m.addr);
  return {};
}

Errc
from_view(View& v, Action_nw_tos& m) {
  from_view(v, m.value);
  pad(v, 3);
  return {};
}

Errc
from_view(View& v, Action_tp_port& m) {
  from_view(v, m.port);
  pad(v, 2);
  return {};
}

Errc
from_view(View& v, Action_vendor& m) {
  from_view(v, m.vendor);
  return {};
}



} // namespace v1_0
} // namespace ofp
} // namespace freeflow
