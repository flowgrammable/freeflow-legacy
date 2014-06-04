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

// -------------------------------------------------------------------------- //
// Bytes

std::size_t
bytes(const Action_payload& m, Action_type t) {
  switch(t) {
  case ACTION_OUTPUT: return bytes(m.output);
  case ACTION_ENQUEUE: return bytes(m.enqueue);
  case ACTION_SET_VLAN_VID: return bytes(m.vlan_vid);
  case ACTION_SET_VLAN_PCP: return bytes(m.vlan_pcp);
  case ACTION_STRIP_VLAN: return bytes(m.empty);
  case ACTION_SET_DL_SRC: return bytes(m.dl_addr);
  case ACTION_SET_DL_DST: return bytes(m.dl_addr);
  case ACTION_SET_NW_SRC: return bytes(m.nw_addr);
  case ACTION_SET_NW_DST: return bytes(m.nw_addr);
  case ACTION_SET_NW_TOS: return bytes(m.nw_tos);
  case ACTION_SET_TP_SRC: return bytes(m.tp_port);
  case ACTION_SET_TP_DST: return bytes(m.tp_port);
  case ACTION_VENDOR: return bytes(m.vendor);
  default:
    throw make_error_code(errc::bad_action_type);
  }
}

// -------------------------------------------------------------------------- //
// To view

Error
to_view(View& v, const Action_empty& m) { return {}; }

Error
to_view(View& v, const Action_output& m) {
  to_view(v, m.port);
  to_view(v, m.max_len);
  return {};
}

Error
to_view(View& v, const Action_enqueue& m) {
  to_view(v, m.port);
  pad(v, 6);
  to_view(v, m.queue);
  return {};
}

Error
to_view(View& v, const Action_vlan_vid& m) {
  to_view(v, m.value);
  pad(v, 2);
  return {};
}

Error
to_view(View& v, const Action_vlan_pcp& m) {
  to_view(v, m.value);
  pad(v, 3);
  return {};
}

Error
to_view(View& v, const Action_dl_addr& m) {
  to_view(v, m.addr);
  pad(v, 6);
  return {};
}

Error
to_view(View& v, const Action_nw_addr& m) {
  to_view(v, m.addr);
  return {};
}

Error
to_view(View& v, const Action_nw_tos& m) {
  to_view(v, m.value);
  pad(v, 3);
  return {};
}

Error
to_view(View& v, const Action_tp_port& m) {
  to_view(v, m.port);
  pad(v, 2);
  return {};
}

Error
to_view(View& v, const Action_vendor& m) {
  to_view(v, m.vendor);
  return {};
}

Error
to_view(View& v, const Action_header& m) {
  if (not is_valid(m.type)) // Required semantic check
    return make_error_code(errc::bad_action_type);
  if (m.length < bytes(m)) // Required semantic check
    return make_error_code(errc::bad_action_length);
  to_view(v, m.type);
  to_view(v, m.length);
  return {};
}

Error
to_view(View& v, const Action_payload& m, Action_type t) {
  switch(t) {
  case ACTION_OUTPUT: return to_view(v, m.output);
  case ACTION_ENQUEUE: return to_view(v, m.enqueue);
  case ACTION_SET_VLAN_VID: return to_view(v, m.vlan_vid);
  case ACTION_SET_VLAN_PCP: return to_view(v, m.vlan_pcp);
  case ACTION_STRIP_VLAN: return to_view(v, m.empty);
  case ACTION_SET_DL_SRC: return to_view(v, m.dl_addr);
  case ACTION_SET_DL_DST: return to_view(v, m.dl_addr);
  case ACTION_SET_NW_SRC: return to_view(v, m.nw_addr);
  case ACTION_SET_NW_DST: return to_view(v, m.nw_addr);
  case ACTION_SET_NW_TOS: return to_view(v, m.nw_tos);
  case ACTION_SET_TP_SRC: return to_view(v, m.tp_port);
  case ACTION_SET_TP_DST: return to_view(v, m.tp_port);
  case ACTION_VENDOR: return to_view(v, m.vendor);
  default:
    throw make_error_code(errc::bad_action_type);
  }
}

Error
to_view(View& v, const Action& m) {
  if (remaining(v) < bytes(m))
    return make_error_code(errc::action_overflow);

  if (Trap err = to_view(v, m.header))
    return err.code();

  if (Constrained_view c = constrain(v, payload_bytes(m))) {
    if (Trap err = to_view(c, m.payload, m.header.type))
      return err.code();
  } else {
    return make_error_code(errc::action_overflow);
  }

  return {};
}

// -------------------------------------------------------------------------- //
// From view

Error
from_view(View& v, Action_empty& m) { return {}; }

Error
from_view(View& v, Action_output& m) {
  from_view(v, m.port);
  from_view(v, m.max_len);
  return {};
}

Error
from_view(View& v, Action_enqueue& m) {
  from_view(v, m.port);
  pad(v, 6);
  from_view(v, m.queue);
  return {};
}

Error
from_view(View& v, Action_vlan_vid& m) {
  from_view(v, m.value);
  pad(v, 2);
  return {};
}

Error
from_view(View& v, Action_vlan_pcp& m) {
  from_view(v, m.value);
  pad(v, 3);
  return {};
}

Error
from_view(View& v, Action_dl_addr& m) {
  from_view(v, m.addr);
  pad(v, 6);
  return {};
}

Error
from_view(View& v, Action_nw_addr& m) {
  from_view(v, m.addr);
  return {};
}

Error
from_view(View& v, Action_nw_tos& m) {
  from_view(v, m.value);
  pad(v, 3);
  return {};
}

Error
from_view(View& v, Action_tp_port& m) {
  from_view(v, m.port);
  pad(v, 2);
  return {};
}

Error
from_view(View& v, Action_vendor& m) {
  from_view(v, m.vendor);
  return {};
}

Error
from_view(View& v, Action_header& m) {
  from_view(v, m.type);
  from_view(v, m.length);
  if (not is_valid(m.type)) // Required semantic check
    return make_error_code(errc::bad_action_type);
  if (m.length < bytes(m)) // Required semantic check
    return make_error_code(errc::bad_action_length);
  return {};
}

Error
from_view(View& v, Action_payload& m, Action_type t) {
  switch(t) {
  case ACTION_OUTPUT: return from_view(v, m.output);
  case ACTION_ENQUEUE: return from_view(v, m.enqueue);
  case ACTION_SET_VLAN_VID: return from_view(v, m.vlan_vid);
  case ACTION_SET_VLAN_PCP: return from_view(v, m.vlan_pcp);
  case ACTION_STRIP_VLAN: return from_view(v, m.empty);
  case ACTION_SET_DL_SRC: return from_view(v, m.dl_addr);
  case ACTION_SET_DL_DST: return from_view(v, m.dl_addr);
  case ACTION_SET_NW_SRC: return from_view(v, m.nw_addr);
  case ACTION_SET_NW_DST: return from_view(v, m.nw_addr);
  case ACTION_SET_NW_TOS: return from_view(v, m.nw_tos);
  case ACTION_SET_TP_SRC: return from_view(v, m.tp_port);
  case ACTION_SET_TP_DST: return from_view(v, m.tp_port);
  case ACTION_VENDOR: return from_view(v, m.vendor);
  default:
    throw make_error_code(errc::bad_action_type);
  }
}

Error
from_view(View& v, Action& m) {
  if (remaining(v) < bytes(m))
    return make_error_code(errc::action_overflow);

  if (Trap err = from_view(v, m.header))
    return err.code();

  if (Constrained_view c = constrain(v, payload_bytes(m))) {
    if (Trap err = from_view(c, m.payload, m.header.type))
      return err.code();
  } else {
    return make_error_code(errc::action_overflow);
  }

  return {};
}

} // namespace v1_0
} // namespace ofp
} // namespace freeflow
