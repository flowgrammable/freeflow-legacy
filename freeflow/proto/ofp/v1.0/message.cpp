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
// Bytes

std::size_t
bytes(const Message& m) {
  const Message::Payload& p = m.payload;
  switch(m.type) {
  case HELLO: return bytes(p.hello);
  case ERROR: return bytes(p.error);
  case ECHO_REQUEST: return bytes(p.echo);
  case ECHO_REPLY: return bytes(p.echo);
  case VENDOR: return bytes(p.vendor);
  case FEATURE_REQUEST: return bytes(p.empty);
  case FEATURE_REPLY: return bytes(p.feature);
  case GET_CONFIG_REQUEST: return bytes(p.empty);
  case GET_CONFIG_REPLY: return bytes(p.config);
  case SET_CONFIG: return bytes(p.config);
  case PACKET_IN: return bytes(p.packet_in);
  case FLOW_REMOVED: return bytes(p.flow_removed);
  case PORT_STATUS: return bytes(p.port_status);
  case PACKET_OUT: return bytes(p.packet_out);
  case FLOW_MOD: return bytes(p.flow_mod);
  case PORT_MOD: return bytes(p.port_mod);
  case STATS_REQUEST: return bytes(p.stats_request);
  case STATS_REPLY: return bytes(p.stats_reply);
  case BARRIER_REQUEST: return bytes(p.empty);
  case BARRIER_REPLY: return bytes(p.empty);
  /*
  case QUEUE_GET_CONFIG_REQUEST: return bytes(p.queue_get_config_request);
  case QUEUE_GET_CONFIG_REPLY: return bytes(p.queue_get_config_reply);
  */
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
to_view(View& v, const Message& m) {
  const Message::Payload& p = m.payload;
  switch(m.type) {
  case HELLO: return to_view(v, p.hello);
  case ERROR: return to_view(v, p.error);
  case ECHO_REQUEST: return to_view(v, p.echo);
  case ECHO_REPLY: return to_view(v, p.echo);
  case VENDOR: return to_view(v, p.vendor);
  case FEATURE_REQUEST: return to_view(v, p.empty);
  case FEATURE_REPLY: return to_view(v, p.feature);
  case GET_CONFIG_REQUEST: return to_view(v, p.empty);
  case GET_CONFIG_REPLY: return to_view(v, p.config);
  case SET_CONFIG: return to_view(v, p.config);
  case PACKET_IN: return to_view(v, p.packet_in);
  case FLOW_REMOVED: return to_view(v, p.flow_removed);
  case PORT_STATUS: return to_view(v, p.port_status);
  case PACKET_OUT: return to_view(v, p.packet_out);
  case FLOW_MOD: return to_view(v, p.flow_mod);
  case PORT_MOD: return to_view(v, p.port_mod);
  case STATS_REQUEST: return to_view(v, p.stats_request);
  case STATS_REPLY: return to_view(v, p.stats_reply);
  case BARRIER_REQUEST: return to_view(v, p.empty);
  case BARRIER_REPLY: return to_view(v, p.empty);
  /*
  case QUEUE_GET_CONFIG_REQUEST: return to_view(v, p.queue_get_config_request);
  case QUEUE_GET_CONFIG_REPLY: return to_view(v, p.queue_get_config_reply);
  */
  default:
    return Errc::BAD_MESSAGE_TYPE;
  }
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
from_view(View& v, Message& m) {
  Message::Payload& p = m.payload;
  switch(m.type) {
  case HELLO: return from_view(v, p.hello);
  case ERROR: return from_view(v, p.error);
  case ECHO_REQUEST: return from_view(v, p.echo);
  case ECHO_REPLY: return from_view(v, p.echo);
  case VENDOR: return from_view(v, p.vendor);
  case FEATURE_REQUEST: return from_view(v, p.empty);
  case FEATURE_REPLY: return from_view(v, p.feature);
  case GET_CONFIG_REQUEST: return from_view(v, p.empty);
  case GET_CONFIG_REPLY: return from_view(v, p.config);
  case SET_CONFIG: return from_view(v, p.config);
  case PACKET_IN: return from_view(v, p.packet_in);
  case FLOW_REMOVED: return from_view(v, p.flow_removed);
  case PORT_STATUS: return from_view(v, p.port_status);
  case PACKET_OUT: return from_view(v, p.packet_out);
  case FLOW_MOD: return from_view(v, p.flow_mod);
  case PORT_MOD: return from_view(v, p.port_mod);
  case STATS_REQUEST: return from_view(v, p.stats_request);
  case STATS_REPLY: return from_view(v, p.stats_reply);
  case BARRIER_REQUEST: return from_view(v, p.empty);
  case BARRIER_REPLY: return from_view(v, p.empty);
  /*
  case QUEUE_GET_CONFIG_REQUEST: return from_view(v, p.queue_get_config_request);
  case QUEUE_GET_CONFIG_REPLY: return from_view(v, p.queue_get_config_reply);
  */
  default:
    return Errc::BAD_MESSAGE_TYPE;
  }
}

// -------------------------------------------------------------------------- //
// Message

Message::Message(Type t)
  : type(t) 
{
  Payload& p = payload;
  switch(type) {
  case HELLO: new (&p.hello) Hello(); break;
  case ERROR: new (&p.error) Error(); break;
  case ECHO_REQUEST: new (&p.echo) Echo(); break;
  case ECHO_REPLY: new (&p.echo) Echo(); break;
  case VENDOR: new (&p.vendor) Vendor(); break;
  case FEATURE_REQUEST: new (&p.empty) Empty(); break;
  case FEATURE_REPLY: new (&p.feature) Feature(); break;
  case GET_CONFIG_REQUEST: new (&p.empty) Empty(); break;
  case GET_CONFIG_REPLY: new (&p.config) Config(); break;
  case SET_CONFIG: new (&p.config) Config(); break;
  case PACKET_IN: new (&p.packet_in) Packet_in(); break;
  case FLOW_REMOVED: new (&p.flow_removed) Flow_removed(); break;
  case PORT_STATUS: new (&p.port_status) Port_status(); break;
  case PACKET_OUT: new (&p.packet_out) Packet_out(); break;
  case FLOW_MOD: new (&p.flow_mod) Flow_mod(); break;
  case PORT_MOD: new (&p.port_mod) Port_mod(); break;
  case STATS_REQUEST: new (&p.stats_request) Stats_request(); break;
  case STATS_REPLY: new (&p.stats_reply) Stats_reply(); break;
  case BARRIER_REQUEST: new (&p.empty) Empty(); break;
  case BARRIER_REPLY: new (&p.empty) Empty(); break;
  /*
  case QUEUE_GET_CONFIG_REQUEST: new (&p.queue_get_config_request) Queue_get_config_request(); break;
  case QUEUE_GET_CONFIG_REPLY: new (&p.queue_get_config_reply) Queue_get_config_reply(); break;
  */
  default:
    throw Errc(Errc::BAD_MESSAGE_TYPE);
  }
}

Message::~Message() {
  Payload& p = payload;
  switch(type) {
  case HELLO: p.hello.~Hello(); break;
  case ERROR: p.error.~Error(); break;
  case ECHO_REQUEST: p.echo.~Echo(); break;
  case ECHO_REPLY: p.echo.~Echo(); break;
  case VENDOR: p.vendor.~Vendor(); break;
  case FEATURE_REQUEST: p.empty.~Empty(); break;
  case FEATURE_REPLY: p.feature.~Feature(); break;
  case GET_CONFIG_REQUEST: p.empty.~Empty(); break;
  case GET_CONFIG_REPLY: p.config.~Config(); break;
  case SET_CONFIG: p.config.~Config(); break;
  case PACKET_IN: p.packet_in.~Packet_in(); break;
  case FLOW_REMOVED: p.flow_removed.~Flow_removed(); break;
  case PORT_STATUS: p.port_status.~Port_status(); break;
  case PACKET_OUT: p.packet_out.~Packet_out(); break;
  case FLOW_MOD: p.flow_mod.~Flow_mod(); break;
  case PORT_MOD: p.port_mod.~Port_mod(); break;
  case STATS_REQUEST: p.stats_request.~Stats_request(); break;
  case STATS_REPLY: p.stats_reply.~Stats_reply(); break;
  case BARRIER_REQUEST: p.empty.~Empty(); break;
  case BARRIER_REPLY: p.empty.~Empty(); break;
  /*
  case QUEUE_GET_CONFIG_REQUEST: p.queue_get_config_request.~Queue_get_config_request(); break;
  case QUEUE_GET_CONFIG_REPLY: p.queue_get_config_reply.~Queue_get_config_reply(); break;
  */
  default:
    throw Errc(Errc::BAD_MESSAGE_TYPE);
  }
}

} // namespace v1_0
} // namespace ofp
} // namespace freeflow
