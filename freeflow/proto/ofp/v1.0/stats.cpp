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

#include "stats.hpp"

namespace freeflow {
namespace ofp {
namespace v1_0 {

// Operations
void 
construct(Stats_reply_payload& m, Stats_type t) {
  switch(t) {
  case STATS_DESC: new (&m.desc) Description_stats(); break;
  case STATS_FLOW: new (&m.flow) Flow_stats(); break;
  case STATS_AGGREGATE: new (&m.aggr) Aggregate_stats(); break;
  case STATS_TABLE: new (&m.table) Table_stats(); break;
  case STATS_PORT: new (&m.port) Port_stats(); break;
  case STATS_QUEUE: new (&m.queue) Queue_stats(); break;
  case STATS_VENDOR: new (&m.vendor) Vendor_stats(); break;
  default: break;
  }
  throw make_error_code(errc::bad_stats_type);
}

void 
destroy(Stats_reply_payload& m, Stats_type t) {
  switch(t) {
  case STATS_DESC: m.desc.~Description_stats(); break;
  case STATS_FLOW: m.flow.~Flow_stats(); break;
  case STATS_AGGREGATE: m.aggr.~Aggregate_stats(); break;
  case STATS_TABLE: m.table.~Table_stats(); break;
  case STATS_PORT: m.port.~Port_stats(); break;
  case STATS_QUEUE: m.queue.~Queue_stats(); break;
  case STATS_VENDOR: m.vendor.~Vendor_stats(); break;
  default: break;
  }
  throw make_error_code(errc::bad_stats_type);
}


// -------------------------------------------------------------------------- //
// Bytes

std::size_t 
bytes(const Stats_request_payload& m, Stats_type t) {
  switch(t) {
  case STATS_DESC: return bytes(m.empty);
  case STATS_FLOW: return bytes(m.flow);
  case STATS_AGGREGATE: return bytes(m.flow);
  case STATS_TABLE: return bytes(m.empty);
  case STATS_PORT: return bytes(m.port);
  case STATS_QUEUE: return bytes(m.queue);
  case STATS_VENDOR: return bytes(m.vendor);
  default: break;
  }
  throw make_error_code(errc::bad_stats_type);
}

std::size_t 
bytes(const Stats_reply_payload& m, Stats_type t) {
  switch(t) {
  case STATS_DESC: return bytes(m.desc);
  case STATS_FLOW: return bytes(m.flow);
  case STATS_AGGREGATE: return bytes(m.aggr);
  case STATS_TABLE: return bytes(m.table);
  case STATS_PORT: return bytes(m.port);
  case STATS_QUEUE: return bytes(m.queue);
  case STATS_VENDOR: return bytes(m.vendor);
  default: break;
  }
  throw make_error_code(errc::bad_stats_type);
}

// -------------------------------------------------------------------------- //
// To view

Error 
to_view(View& v, const Flow_stats_request& m) {
  to_view(v, m.match);
  to_view(v, m.table_id);
  pad(v, 1);
  to_view(v, m.out_port);
  return {};
}

Error 
to_view(View& v, const Port_stats_request& m) {
  to_view(v, m.port_number);
  pad(v, 6);
  return {};
}

Error 
to_view(View& v, const Queue_stats_request& m) {
  to_view(v, m.port_number);
  pad(v, 2);
  to_view(v, m.queue_id);
  return {};
}

Error 
to_view(View& v, const Vendor_stats_request& m) {
  to_view(v, m.vendor_id);
  return {};
}

Error 
to_view(View& v, const Description_stats& m) {
  to_view(v, m.mfr_desc);
  to_view(v, m.hw_desc);
  to_view(v, m.sw_desc);
  to_view(v, m.serial_number);
  to_view(v, m.dp_desc);
  return {};
}

Error 
to_view(View& v, const Flow_stats_entry& m) {
  if (m.length < bytes(m)) // Required semantic check
    return make_error_code(errc::bad_flow_stats_length);

  to_view(v, m.length);
  to_view(v, m.table_id);
  pad(v, 1);
  to_view(v, m.match);
  to_view(v, m.duration_sec);
  to_view(v, m.duration_nsec);
  to_view(v, m.priority);
  to_view(v, m.idle_timeout);
  to_view(v, m.hard_timeout);
  pad(v, 6);
  to_view(v, m.cookie);
  to_view(v, m.packet_count);
  to_view(v, m.byte_count);

  if (Constrained_view c = constrain(v, m.length - bytes(m)))
    return to_view(c, m.actions);
  return
    make_error_code(errc::flow_stats_overflow);
}

Error 
to_view(View& v, const Aggregate_stats& m) {
  to_view(v, m.packet_count);
  to_view(v, m.byte_count);
  to_view(v, m.flow_count);
  pad(v, 4);
  return {};
}

Error 
to_view(View& v, const Table_stats_entry& m) {
  to_view(v, m.table_id);
  pad(v, 3);
  to_view(v, m.name);
  to_view(v, m.wildcards);
  to_view(v, m.max_entries);
  to_view(v, m.active_count);
  to_view(v, m.lookup_count);
  to_view(v, m.matched_count);
  return {};
}

Error 
to_view(View& v, const Port_stats_entry& m) {
  to_view(v, m.port_number);
  pad(v, 6);
  to_view(v, m.rx_packets);
  to_view(v, m.tx_packets);
  to_view(v, m.rx_bytes);
  to_view(v, m.tx_bytes);
  to_view(v, m.rx_dropped);
  to_view(v, m.tx_dropped);
  to_view(v, m.rx_errors);
  to_view(v, m.tx_errors);
  to_view(v, m.rx_frame_err);
  to_view(v, m.rx_over_err);
  to_view(v, m.rx_crc_err);
  to_view(v, m.collisions);
  return {};
}

Error 
to_view(View& v, const Queue_stats_entry& m) {
  if (m.length != 32) // Required semantic check
    return make_error_code(errc::bad_queue_stats_length);
  to_view(v, m.length);
  pad(v, 2);
  to_view(v, m.tx_bytes);
  to_view(v, m.tx_packets);
  to_view(v, m.tx_errors);
  return {};
}

Error 
to_view(View& v, const Vendor_stats& m) {
  if (remaining(v) < bytes(m))
    return make_error_code(errc::vendor_stats_overflow);
  to_view(v, m.vendor_id);
  to_view(v, m.data);
  return {};
}

Error 
to_view(View& v, const Stats_header& m) {
  if (not is_valid(m.type)) // Required semantic check
    return make_error_code(errc::bad_stats_type);
  to_view(v, m.type);
  to_view(v, m.flags);
  return {};
}

Error
to_view(View& v, const Stats_request_payload& m, Stats_type t) {
  switch(t) {
  case STATS_DESC: return to_view(v, m.empty);
  case STATS_FLOW: return to_view(v, m.flow);
  case STATS_AGGREGATE: return to_view(v, m.flow);
  case STATS_TABLE: return to_view(v, m.empty);
  case STATS_PORT: return to_view(v, m.port);
  case STATS_QUEUE: return to_view(v, m.queue);
  case STATS_VENDOR: return to_view(v, m.vendor);  
  default: break;
  }
  throw make_error_code(errc::bad_stats_type);
}

Error
to_view(View& v, const Stats_reply_payload& m, Stats_type t) {
  switch(t) {
  case STATS_DESC: return to_view(v, m.desc);
  case STATS_FLOW: return to_view(v, m.flow);
  case STATS_AGGREGATE: return to_view(v, m.aggr);
  case STATS_TABLE: return to_view(v, m.table);
  case STATS_PORT: return to_view(v, m.port);
  case STATS_QUEUE: return to_view(v, m.queue);
  case STATS_VENDOR: return to_view(v, m.vendor);  
  default: break;
  }
  throw make_error_code(errc::bad_stats_type);
}

// -------------------------------------------------------------------------- //
// From view

Error 
from_view(View& v, Flow_stats_request& m) {
  from_view(v, m.match);
  from_view(v, m.table_id);
  pad(v, 1);
  from_view(v, m.out_port);
  return {};
}

Error 
from_view(View& v, Port_stats_request& m) {
  from_view(v, m.port_number);
  pad(v, 6);
  return {};
}

Error 
from_view(View& v, Queue_stats_request& m) {
  from_view(v, m.port_number);
  pad(v, 2);
  from_view(v, m.queue_id);
  return {};
}

Error 
from_view(View& v, Vendor_stats_request& m) {
  from_view(v, m.vendor_id);
  return {};
}

Error 
from_view(View& v, Description_stats& m) {
  from_view(v, m.mfr_desc);
  from_view(v, m.hw_desc);
  from_view(v, m.sw_desc);
  from_view(v, m.serial_number);
  from_view(v, m.dp_desc);
  return {};
}

Error 
from_view(View& v, Flow_stats_entry& m) {
  from_view(v, m.length);
  from_view(v, m.table_id);
  pad(v, 1);
  from_view(v, m.match);
  from_view(v, m.duration_sec);
  from_view(v, m.duration_nsec);
  from_view(v, m.priority);
  from_view(v, m.idle_timeout);
  from_view(v, m.hard_timeout);
  pad(v, 6);
  from_view(v, m.cookie);
  from_view(v, m.packet_count);
  from_view(v, m.byte_count);

  if (m.length < bytes(m)) // Required semantic check
    return make_error_code(errc::bad_flow_stats_length);

  if (Constrained_view c = constrain(v, m.length - bytes(m)))
    return from_view(c, m.actions);
  return
    make_error_code(errc::flow_stats_overflow);
}

Error 
from_view(View& v, Aggregate_stats& m) {
  from_view(v, m.packet_count);
  from_view(v, m.byte_count);
  from_view(v, m.flow_count);
  pad(v, 4);
  return {};
}

Error 
from_view(View& v, Table_stats_entry& m) {
  from_view(v, m.table_id);
  pad(v, 3);
  from_view(v, m.name);
  from_view(v, m.wildcards);
  from_view(v, m.max_entries);
  from_view(v, m.active_count);
  from_view(v, m.lookup_count);
  from_view(v, m.matched_count);
  return {};
}

Error 
from_view(View& v, Port_stats_entry& m) {
  from_view(v, m.port_number);
  pad(v, 6);
  from_view(v, m.rx_packets);
  from_view(v, m.tx_packets);
  from_view(v, m.rx_bytes);
  from_view(v, m.tx_bytes);
  from_view(v, m.rx_dropped);
  from_view(v, m.tx_dropped);
  from_view(v, m.rx_errors);
  from_view(v, m.tx_errors);
  from_view(v, m.rx_frame_err);
  from_view(v, m.rx_over_err);
  from_view(v, m.rx_crc_err);
  from_view(v, m.collisions);
  return {};
}

Error 
from_view(View& v, Queue_stats_entry& m) {
  from_view(v, m.length);
  pad(v, 2);
  from_view(v, m.tx_bytes);
  from_view(v, m.tx_packets);
  from_view(v, m.tx_errors);

  if (m.length != 32) // Required semantic check
    return make_error_code(errc::bad_queue_stats_length);
  return {};
}

Error 
from_view(View& v, Vendor_stats& m) {
  if (remaining(v) < bytes(m))
    return make_error_code(errc::vendor_stats_overflow);
  from_view(v, m.vendor_id);
  from_view(v, m.data);
  return {};
}

Error 
from_view(View& v, Stats_header& m) {
  from_view(v, m.type);
  from_view(v, m.flags);
  if (not is_valid(m.type)) // Required semantic check
    return make_error_code(errc::bad_stats_type);
  return {};
}

Error
from_view(View& v, Stats_request_payload& m, Stats_type t) {
  switch(t) {
  case STATS_DESC: return from_view(v, m.empty);
  case STATS_FLOW: return from_view(v, m.flow);
  case STATS_AGGREGATE: return from_view(v, m.flow);
  case STATS_TABLE: return from_view(v, m.empty);
  case STATS_PORT: return from_view(v, m.port);
  case STATS_QUEUE: return from_view(v, m.queue);
  case STATS_VENDOR: return from_view(v, m.vendor);  
  default: break;
  }
  throw make_error_code(errc::bad_stats_type);
}

Error
from_view(View& v, Stats_reply_payload& m, Stats_type t) {
  construct(m, t); // Guarantee that non-trivial members are valid
  switch(t) {
  case STATS_DESC: return from_view(v, m.desc);
  case STATS_FLOW: return from_view(v, m.flow);
  case STATS_AGGREGATE: return from_view(v, m.aggr);
  case STATS_TABLE: return from_view(v, m.table);
  case STATS_PORT: return from_view(v, m.port);
  case STATS_QUEUE: return from_view(v, m.queue);
  case STATS_VENDOR: return from_view(v, m.vendor);  
  default: break;
  }
  throw make_error_code(errc::bad_stats_type);
}

} // namespace v1_0
} // namespace ofp
} // namespace freeflow
