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

#include "message.hpp"

namespace freeflow {
namespace ofp {
namespace v1_0 {

// -------------------------------------------------------------------------- //
// Operations

// NOTE: We don't have to explicitly construct trivial members!
void 
construct(Payload& m, Message_type t) {
  switch(t) {
  case HELLO: new (&m.hello) Hello();
  case ERROR: new (&m.error) Error();
  case ECHO_REQUEST: new (&m.echo) Echo();
  case ECHO_REPLY: new (&m.echo) Echo();
  case VENDOR: new (&m.vendor) Vendor();
  case FEATURE_REQUEST: new (&m.empty) Empty();
  case FEATURE_REPLY: new (&m.feature) Feature();
  case GET_CONFIG_REQUEST: new (&m.empty) Empty();
  case GET_CONFIG_REPLY: new (&m.config) Config();
  case SET_CONFIG: new (&m.config) Config();
  case PACKET_IN: new (&m.packet_in) Packet_in();
  case FLOW_REMOVED: new (&m.flow_removed) Flow_removed();
  case PORT_STATUS: new (&m.port_status) Port_status();
  case PACKET_OUT: new (&m.packet_out) Packet_out();
  case FLOW_MOD: new (&m.flow_mod) Flow_mod();
  case PORT_MOD: new (&m.port_mod) Port_mod();
  case STATS_REQUEST: new (&m.stats_request) Stats_request();
  case STATS_REPLY: new (&m.stats_reply) Stats_reply();
  case BARRIER_REQUEST: new (&m.empty) Empty();
  case BARRIER_REPLY: new (&m.empty) Empty();
  case QUEUE_GET_CONFIG_REQUEST: new (&m.queue_config_request) Queue_config_request();
  case QUEUE_GET_CONFIG_REPLY: new (&m.queue_config_reply) Queue_config_reply();
  default: throw Errc(Errc::BAD_MESSAGE_TYPE);
  }
}

// NOTE: We don't have to explicitly destroy trivial members!
void 
destroy(Payload& m, Message_type t) {
  switch(t) {
  case HELLO: m.hello.~Hello(); break;
  case ERROR: m.error.~Error(); break;
  case ECHO_REQUEST: m.echo.~Echo(); break;
  case ECHO_REPLY: m.echo.~Echo(); break;
  case VENDOR: m.vendor.~Vendor(); break;
  case FEATURE_REQUEST: m.empty.~Empty(); break;
  case FEATURE_REPLY: m.feature.~Feature(); break;
  case GET_CONFIG_REQUEST: m.empty.~Empty(); break;
  case GET_CONFIG_REPLY: m.config.~Config(); break;
  case SET_CONFIG: m.config.~Config(); break;
  case PACKET_IN: m.packet_in.~Packet_in(); break;
  case FLOW_REMOVED: m.flow_removed.~Flow_removed(); break;
  case PORT_STATUS: m.port_status.~Port_status(); break;
  case PACKET_OUT: m.packet_out.~Packet_out(); break;
  case FLOW_MOD: m.flow_mod.~Flow_mod(); break;
  case PORT_MOD: m.port_mod.~Port_mod(); break;
  case STATS_REQUEST: m.stats_request.~Stats_request(); break;
  case STATS_REPLY: m.stats_reply.~Stats_reply(); break;
  case BARRIER_REQUEST: m.empty.~Empty(); break;
  case BARRIER_REPLY: m.empty.~Empty(); break;
  case QUEUE_GET_CONFIG_REQUEST: m.queue_config_request.~Queue_config_request(); break;
  case QUEUE_GET_CONFIG_REPLY: m.queue_config_reply.~Queue_config_reply(); break;
  default: throw Errc(Errc::BAD_MESSAGE_TYPE);
  }
}


// -------------------------------------------------------------------------- //
// Bytes

std::size_t
bytes(const Payload& m, Message_type t) {
  switch(t) {
  case HELLO: return bytes(m.hello);
  case ERROR: return bytes(m.error);
  case ECHO_REQUEST: return bytes(m.echo);
  case ECHO_REPLY: return bytes(m.echo);
  case VENDOR: return bytes(m.vendor);
  case FEATURE_REQUEST: return bytes(m.empty);
  case FEATURE_REPLY: return bytes(m.feature);
  case GET_CONFIG_REQUEST: return bytes(m.empty);
  case GET_CONFIG_REPLY: return bytes(m.config);
  case SET_CONFIG: return bytes(m.config);
  case PACKET_IN: return bytes(m.packet_in);
  case FLOW_REMOVED: return bytes(m.flow_removed);
  case PORT_STATUS: return bytes(m.port_status);
  case PACKET_OUT: return bytes(m.packet_out);
  case FLOW_MOD: return bytes(m.flow_mod);
  case PORT_MOD: return bytes(m.port_mod);
  case STATS_REQUEST: return bytes(m.stats_request);
  case STATS_REPLY: return bytes(m.stats_reply);
  case BARRIER_REQUEST: return bytes(m.empty);
  case BARRIER_REPLY: return bytes(m.empty);
  case QUEUE_GET_CONFIG_REQUEST: return bytes(m.queue_config_request);
  case QUEUE_GET_CONFIG_REPLY: return bytes(m.queue_config_reply);
  default:
    throw Errc(Errc::BAD_MESSAGE_TYPE);
  }
}

// -------------------------------------------------------------------------- //
// To view

Errc
to_view(View& v, const Hello& m) {
  if (remaining(v) < bytes(m))
    return Errc::HELLO_OVERFLOW;
  to_view(v, m.data);
  return {};
}

Errc
to_view(View& v, const Error& m) {
  if (remaining(v) < bytes(m))
    return Errc::ERROR_OVERFLOW;
  to_view(v, m.type);
  to_view(v, m.code);
  to_view(v, m.data);
  return {};
}

Errc
to_view(View& v, const Echo& m) {
  if (remaining(v) < bytes(m))
    return Errc::HELLO_OVERFLOW;
  to_view(v, m.data);
  return {};
}

Errc
to_view(View& v, const Vendor& m) {
  if (remaining(v) < bytes(m))
    return Errc::VENDOR_OVERFLOW;
  to_view(v, m.vendor_id);
  to_view(v, m.data);
  return {};
}

Errc
to_view(View& v, const Feature& m) {
  if (remaining(v) < bytes(m))
    return Errc::FEATURE_OVERFLOW;
  to_view(v, m.datapath_id);
  to_view(v, m.nbuffers);
  to_view(v, m.ntables);
  pad(v, 3);
  to_view(v, m.capabilities);
  to_view(v, m.actions);
  to_view(v, m.ports);
  return {};
}

Errc
to_view(View& v, const Config& m) {
  if (remaining(v) < bytes(m))
    return Errc::CONFIG_OVERFLOW;
  to_view(v, m.flags);
  to_view(v, m.miss_send_len);
  return {};
}

Errc
to_view(View& v, const Packet_in& m) {
  if (remaining(v) < bytes(m))
    return Errc::PACKET_IN_OVERFLOW;
  to_view(v, m.buffer_id);
  to_view(v, m.total_len);
  to_view(v, m.in_port);
  to_view(v, m.reason);
  pad(v, 1);
  to_view(v, m.data);
  return {};
}

Errc
to_view(View& v, const Flow_removed& m) {
  if (remaining(v) < bytes(m))
    return Errc::FLOW_REMOVED_OVERFLOW;
  to_view(v, m.match);
  to_view(v, m.cookie);
  to_view(v, m.priority);
  to_view(v, m.reason);
  pad(v, 1);
  to_view(v, m.duration_sec);
  to_view(v, m.duration_nsec);
  to_view(v, m.idle_timeout);
  pad(v, 2);
  to_view(v, m.packet_count);
  to_view(v, m.byte_count);
  return {};
}

Errc
to_view(View& v, const Port_status& m) {
  if (remaining(v) < bytes(m))
    return Errc::PACKET_OUT_OVERFLOW;
  to_view(v, m.reason);
  pad(v, 7);
  to_view(v, m.port);
  return {};
}

Errc
to_view(View& v, const Packet_out& m) {
  if (remaining(v) < bytes(m))
    return Errc::PACKET_OUT_OVERFLOW;

  to_view(v, m.buffer_id);
  to_view(v, m.port);
  to_view(v, m.actions_len);
  
  if (Constrained_view c = constrain(v, m.actions_len)) {
    if (Trap err = to_view(v, m.actions))
      return err.code();
  } else {
    return Errc::PACKET_OUT_OVERFLOW;
  }

  to_view(v, m.data);
  return {};
}

Errc
to_view(View& v, const Flow_mod& m) {
  if (remaining(v) < bytes(m))
    return Errc::FLOW_MOD_OVERFLOW;
  to_view(v, m.match);
  to_view(v, m.cookie);
  to_view(v, m.command);
  to_view(v, m.idle_timeout);
  to_view(v, m.hard_timeout);
  to_view(v, m.priority);
  to_view(v, m.buffer_id);
  to_view(v, m.out_port);
  to_view(v, m.flags);
  return to_view(v, m.actions);
}

Errc
to_view(View& v, const Port_mod& m) {
  if (remaining(v) < bytes(m))
    return Errc::PORT_MOD_OVERFLOW;
  to_view(v, m.port);
  to_view(v, m.hw_addr);
  to_view(v, m.config);
  to_view(v, m.mask);
  to_view(v, m.advertised);
  return {};
}

Errc
to_view(View& v, const Stats_request& m) {
  if (remaining(v) < bytes(m))
    return Errc::STATS_REQUEST_OVERFLOW;
  to_view(v, m.header);
  return to_view(v, m.payload, m.header.type);
}

Errc
to_view(View& v, const Stats_reply& m) {
  if (remaining(v) < bytes(m))
    return Errc::STATS_REPLY_OVERFLOW;
  to_view(v, m.header);
  return to_view(v, m.payload, m.header.type);
}

Errc 
to_view(View& v, const Queue_config_request& m) {
  if (remaining(v) < bytes(m))
    return Errc::QUEUE_CONFIG_REQUEST_OVERFLOW;
  to_view(v, m.port);
  return {};
}

Errc
to_view(View& v, const Queue_config_reply& m) {
  if (remaining(v) < bytes(m))
    return Errc::QUEUE_CONFIG_REPLY_OVERFLOW;
  to_view(v, m.port);
  to_view(v, m.queues);
  return {};
}

Errc
to_view(View& v, const Header& m) {
  if (not is_valid(m.version)) // Optional semantic check
    return Errc::BAD_VERSION;
  if (not is_valid(m.type)) // Required semantic check
    return Errc::BAD_MESSAGE_TYPE;
  if (m.length < bytes(m)) // Required semantic check
    return Errc::BAD_HEADER_LENGTH;
  to_view(v, m.version);
  to_view(v, m.type);
  to_view(v, m.length);
  to_view(v, m.xid);
  return {};
}

Errc
to_view(View& v, const Payload& m, Message_type t) {
  switch(t) {
  case HELLO: return to_view(v, m.hello);
  case ERROR: return to_view(v, m.error);
  case ECHO_REQUEST: return to_view(v, m.echo);
  case ECHO_REPLY: return to_view(v, m.echo);
  case VENDOR: return to_view(v, m.vendor);
  case FEATURE_REQUEST: return to_view(v, m.empty);
  case FEATURE_REPLY: return to_view(v, m.feature);
  case GET_CONFIG_REQUEST: return to_view(v, m.empty);
  case GET_CONFIG_REPLY: return to_view(v, m.config);
  case SET_CONFIG: return to_view(v, m.config);
  case PACKET_IN: return to_view(v, m.packet_in);
  case FLOW_REMOVED: return to_view(v, m.flow_removed);
  case PORT_STATUS: return to_view(v, m.port_status);
  case PACKET_OUT: return to_view(v, m.packet_out);
  case FLOW_MOD: return to_view(v, m.flow_mod);
  case PORT_MOD: return to_view(v, m.port_mod);
  case STATS_REQUEST: return to_view(v, m.stats_request);
  case STATS_REPLY: return to_view(v, m.stats_reply);
  case BARRIER_REQUEST: return to_view(v, m.empty);
  case BARRIER_REPLY: return to_view(v, m.empty);
  case QUEUE_GET_CONFIG_REQUEST: return to_view(v, m.queue_config_request);
  case QUEUE_GET_CONFIG_REPLY: return to_view(v, m.queue_config_reply);
  default: return Errc::BAD_MESSAGE_TYPE;
  }
}

Errc
to_view(View& v, const Message& m) {
  if (remaining(v) < bytes(m))
    return Errc::MESSAGE_OVERFLOW; // FIXME: not the right code?

  to_view(v, m.header);
  
  // TODO: Constrain the view?
  return to_view(v, m.payload, m.header.type);
}


// -------------------------------------------------------------------------- //
// From view

Errc
from_view(View& v, Hello& m) {
  // No overflow check is needed (the buffer can be 0 bytes).
  from_view(v, m.data);
  return {};
}

Errc
from_view(View& v, Error& m) {
  if (remaining(v) < bytes(m))
    return Errc::ERROR_OVERFLOW;
  from_view(v, m.type);
  from_view(v, m.code);
  from_view(v, m.data);
  return {};
}

Errc
from_view(View& v, Echo& m) {
  // No overflow check is needed (the buffer can be 0 bytes).
  from_view(v, m.data);
  return {};
}

Errc
from_view(View& v, Vendor& m) {
  if (remaining(v) < bytes(m))
    return Errc::VENDOR_OVERFLOW;
  from_view(v, m.vendor_id);
  from_view(v, m.data);
  return {};
}

Errc
from_view(View& v, Feature& m) {
  if (remaining(v) < bytes(m))
    return Errc::FEATURE_OVERFLOW;
  from_view(v, m.datapath_id);
  from_view(v, m.nbuffers);
  from_view(v, m.ntables);
  pad(v, 3);
  from_view(v, m.capabilities);
  from_view(v, m.actions);
  from_view(v, m.ports);
  return {};
}

Errc
from_view(View& v, Config& m) {
  if (remaining(v) < bytes(m))
    return Errc::CONFIG_OVERFLOW;
  from_view(v, m.flags);
  from_view(v, m.miss_send_len);
  return {};
}

Errc
from_view(View& v, Packet_in& m) {
  if (remaining(v) < bytes(m))
    return Errc::PACKET_IN_OVERFLOW;
  from_view(v, m.buffer_id);
  from_view(v, m.total_len);
  from_view(v, m.in_port);
  from_view(v, m.reason);
  pad(v, 1);
  from_view(v, m.data);
  return {};
}

Errc
from_view(View& v, Flow_removed& m) {
  if (remaining(v) < bytes(m))
    return Errc::FLOW_REMOVED_OVERFLOW;
  from_view(v, m.match);
  from_view(v, m.cookie);
  from_view(v, m.priority);
  from_view(v, m.reason);
  pad(v, 1);
  from_view(v, m.duration_sec);
  from_view(v, m.duration_nsec);
  from_view(v, m.idle_timeout);
  pad(v, 2);
  from_view(v, m.packet_count);
  from_view(v, m.byte_count);
  return {};
}

Errc
from_view(View& v, Port_status& m) {
  if (remaining(v) < bytes(m))
    return Errc::PORT_STATUS_OVERFLOW;
  from_view(v, m.reason);
  pad(v, 7);
  from_view(v, m.port);
  return {};
}

Errc
from_view(View& v, Packet_out& m) {
  if (remaining(v) < bytes(m))
    return Errc::PACKET_OUT_OVERFLOW;

  from_view(v, m.buffer_id);
  from_view(v, m.port);
  from_view(v, m.actions_len);

  if (Constrained_view c = constrain(v, m.actions_len)) {
    if (Trap err = from_view(v, m.actions))
      return err.code();
  } else {
    return Errc::PACKET_OUT_OVERFLOW;
  }

  from_view(v, m.data);
  return {};
}

Errc
from_view(View& v, Flow_mod& m) {
  if (remaining(v) < bytes(m))
    return Errc::FLOW_MOD_OVERFLOW;
  from_view(v, m.match);
  from_view(v, m.cookie);
  from_view(v, m.command);
  from_view(v, m.idle_timeout);
  from_view(v, m.hard_timeout);
  from_view(v, m.priority);
  from_view(v, m.buffer_id);
  from_view(v, m.out_port);
  from_view(v, m.flags);
  return from_view(v, m.actions);
}

Errc
from_view(View& v, Port_mod& m) {
  if (remaining(v) < bytes(m))
    return Errc::PORT_MOD_OVERFLOW;
  from_view(v, m.port);
  from_view(v, m.hw_addr);
  from_view(v, m.config);
  from_view(v, m.mask);
  from_view(v, m.advertised);
  return {};
}

Errc
from_view(View& v, Stats_request& m) {
  if (remaining(v) < bytes(m))
    return Errc::STATS_REQUEST_OVERFLOW;
  from_view(v, m.header);
  return from_view(v, m.payload, m.header.type);
}

Errc
from_view(View& v, Stats_reply& m) {
  if (remaining(v) < bytes(m))
    return Errc::STATS_REPLY_OVERFLOW;
  from_view(v, m.header);
  return from_view(v, m.payload, m.header.type);
}

Errc 
from_view(View& v, Queue_config_request& m) {
  if (remaining(v) < bytes(m))
    return Errc::QUEUE_CONFIG_REQUEST_OVERFLOW;
  from_view(v, m.port);
  return {};
}

Errc
from_view(View& v, Queue_config_reply& m) {
  if (remaining(v) < bytes(m))
    return Errc::QUEUE_CONFIG_REPLY_OVERFLOW;
  from_view(v, m.port);
  from_view(v, m.queues);
  return {};
}

Errc
from_view(View& v, Header& m) {
  if (remaining(v) < bytes(m))
    return Errc::MESSAGE_OVERFLOW; // FIXME: not the right code?
  from_view(v, m.version);
  from_view(v, m.type);
  from_view(v, m.length);
  from_view(v, m.xid);
  if (not is_valid(m.version)) // Optional semantic check
    return Errc::BAD_VERSION;
  if (not is_valid(m.type)) // Required semantic check
    return Errc::BAD_MESSAGE_TYPE;
  if (m.length < bytes(m)) // Required semantic check
    return Errc::BAD_HEADER_LENGTH;
  return {};
}

Errc
from_view(View& v, Payload& m, Message_type t) {
  construct(m, t); // Guarantee initialization
  switch(t) {
  case HELLO: return from_view(v, m.hello);
  case ERROR: return from_view(v, m.error);
  case ECHO_REQUEST: return from_view(v, m.echo);
  case ECHO_REPLY: return from_view(v, m.echo);
  case VENDOR: return from_view(v, m.vendor);
  case FEATURE_REQUEST: return from_view(v, m.empty);
  case FEATURE_REPLY: return from_view(v, m.feature);
  case GET_CONFIG_REQUEST: return from_view(v, m.empty);
  case GET_CONFIG_REPLY: return from_view(v, m.config);
  case SET_CONFIG: return from_view(v, m.config);
  case PACKET_IN: return from_view(v, m.packet_in);
  case FLOW_REMOVED: return from_view(v, m.flow_removed);
  case PORT_STATUS: return from_view(v, m.port_status);
  case PACKET_OUT: return from_view(v, m.packet_out);
  case FLOW_MOD: return from_view(v, m.flow_mod);
  case PORT_MOD: return from_view(v, m.port_mod);
  case STATS_REQUEST: return from_view(v, m.stats_request);
  case STATS_REPLY: return from_view(v, m.stats_reply);
  case BARRIER_REQUEST: return from_view(v, m.empty);
  case BARRIER_REPLY: return from_view(v, m.empty);
  case QUEUE_GET_CONFIG_REQUEST: return from_view(v, m.queue_config_request);
  case QUEUE_GET_CONFIG_REPLY: return from_view(v, m.queue_config_reply);
  default: return Errc::BAD_MESSAGE_TYPE;
  }
}

Errc
from_view(View& v, Message& m) {
  if (remaining(v) < bytes(m))
    return Errc::MESSAGE_OVERFLOW;

  from_view(v, m.header);

  // TODO: Constrain the view?
  return from_view(v, m.payload, m.header.type);
}

} // namespace v1_0
} // namespace ofp
} // namespace freeflow
