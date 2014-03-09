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

Errc
to_view(View& v, const Hello& m) {
  if (v.remaining() < bytes(m))
    return Errc::HELLO_OVERFLOW;
  to_view(v, m.data);
  return {};
}

Errc
from_view(View& v, Hello& m) {
  // No overflow check is needed (the buffer can be 0 bytes).
  from_view(v, m.data);
  return {};
}


Errc
to_view(View& v, const Error& m) {
  if (v.remaining() < bytes(m))
    return Errc::ERROR_OVERFLOW;
  to_view(v, m.type);
  to_view(v, m.code);
  to_view(v, m.data);
  return {};
}

Errc
from_view(View& v, Error& m) {
  if (v.remaining() < bytes(m))
    return Errc::ERROR_OVERFLOW;
  from_view(v, m.type);
  from_view(v, m.code);
  from_view(v, m.data);
  return {};
}


Errc
to_view(View& v, const Echo& m) {
  if (v.remaining() < bytes(m))
    return Errc::HELLO_OVERFLOW;
  to_view(v, m.data);
  return {};
}

Errc
from_view(View& v, Echo& m) {
  // No overflow check is needed (the buffer can be 0 bytes).
  from_view(v, m.data);
  return {};
}

Message::Message(Type t)
  : type(t) 
{
  Payload& p = payload;
  switch(type) {
  case HELLO: new (&p.hello) Hello(); break;
  case ERROR: new (&p.error) Error(); break;
  case ECHO_REQUEST: new (&p.echo) Echo(); break;
  case ECHO_REPLY: new (&p.echo) Echo(); break;
  /*
  case VENDOR: new (&p.vendor) Vendor(); break;
  case FEATURE_REQUEST: new (&p.feature_request) Feature_request(); break;
  case FEATURE_REPLY: new (&p.feature_reply) Feature_reply(); break;
  case GET_CONFIG_REQUEST: new (&p.get_config_request) Get_config_request(); break;
  case GET_CONFIG_REPLY: new (&p.get_config_reply) Get_config_reply(); break;
  case SET_CONFIG: new (&p.set_config) Set_config(); break;
  case PACKET_IN: new (&p.packet_in) Packet_in(); break;
  case FLOW_REMOVED: new (&p.flow_removed) Flow_removed(); break;
  case PORT_STATUS: new (&p.port_status) Port_status(); break;
  case PACKET_OUT: new (&p.packet_out) Packet_out(); break;
  case FLOW_MOD: new (&p.flow_mod) Flow_mod(); break;
  case PORT_MOD: new (&p.port_mod) Port_mod(); break;
  case STATS_REQUEST: new (&p.stats_request) Stats_request(); break;
  case STATS_REPLY: new (&p.stats_reply) Stats_reply(); break;
  case BARRIER_REQUEST: new (&p.barrier_request) Barrier_request(); break;
  case BARRIER_REPLY: new (&p.barrier_reply) Barrier_reply(); break;
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
  /*
  case VENDOR: p.vendor.~Vendor(); break;
  case FEATURE_REQUEST: p.feature_request.~Feature_request(); break;
  case FEATURE_REPLY: p.feature_reply.~Feature_reply(); break;
  case GET_CONFIG_REQUEST: p.get_config_request.~Get_config_request(); break;
  case GET_CONFIG_REPLY: p.get_config_reply.~Get_config_reply(); break;
  case SET_CONFIG: p.set_config.~Set_config(); break;
  case PACKET_IN: p.packet_in.~Packet_in(); break;
  case FLOW_REMOVED: p.flow_removed.~Flow_removed(); break;
  case PORT_STATUS: p.port_status.~Port_status(); break;
  case PACKET_OUT: p.packet_out.~Packet_out(); break;
  case FLOW_MOD: p.flow_mod.~Flow_mod(); break;
  case PORT_MOD: p.port_mod.~Port_mod(); break;
  case STATS_REQUEST: p.stats_request.~Stats_request(); break;
  case STATS_REPLY: p.stats_reply.~Stats_reply(); break;
  case BARRIER_REQUEST: p.barrier_request.~Barrier_request(); break;
  case BARRIER_REPLY: p.barrier_reply.~Barrier_reply(); break;
  case QUEUE_GET_CONFIG_REQUEST: p.queue_get_config_request.~Queue_get_config_request(); break;
  case QUEUE_GET_CONFIG_REPLY: p.queue_get_config_reply.~Queue_get_config_reply(); break;
  */
  default:
    throw Errc(Errc::BAD_MESSAGE_TYPE);
  }
}

std::size_t
bytes(const Message& m) {
  const Message::Payload& p = m.payload;
  switch(m.type) {
  case HELLO: return bytes(p.hello);
  case ERROR: return bytes(p.error);
  case ECHO_REQUEST: return bytes(p.echo);
  case ECHO_REPLY: return bytes(p.echo);
  /*
  case VENDOR: return bytes(p.vendor);
  case FEATURE_REQUEST: return bytes(p.feature_request);
  case FEATURE_REPLY: return bytes(p.feature_reply);
  case GET_CONFIG_REQUEST: return bytes(p.get_config_request);
  case GET_CONFIG_REPLY: return bytes(p.get_config_reply);
  case SET_CONFIG: return bytes(p.set_config);
  case PACKET_IN: return bytes(p.packet_in);
  case FLOW_REMOVED: return bytes(p.flow_removed);
  case PORT_STATUS: return bytes(p.port_status);
  case PACKET_OUT: return bytes(p.packet_out);
  case FLOW_MOD: return bytes(p.flow_mod);
  case PORT_MOD: return bytes(p.port_mod);
  case STATS_REQUEST: return bytes(p.stats_request);
  case STATS_REPLY: return bytes(p.stats_reply);
  case BARRIER_REQUEST: return bytes(p.barrier_request);
  case BARRIER_REPLY: return bytes(p.barrier_reply);
  case QUEUE_GET_CONFIG_REQUEST: return bytes(p.queue_get_config_request);
  case QUEUE_GET_CONFIG_REPLY: return bytes(p.queue_get_config_reply);
  */
  default:
    throw Errc(Errc::BAD_MESSAGE_TYPE);
  }
}

Errc
to_view(View& v, const Message& m) {
  const Message::Payload& p = m.payload;
  switch(m.type) {
  case HELLO: return to_view(v, p.hello);
  case ERROR: return to_view(v, p.error);
  case ECHO_REQUEST: return to_view(v, p.echo);
  case ECHO_REPLY: return to_view(v, p.echo);
  /*
  case VENDOR: return to_view(v, p.vendor);
  case FEATURE_REQUEST: return to_view(v, p.feature_request);
  case FEATURE_REPLY: return to_view(v, p.feature_reply);
  case GET_CONFIG_REQUEST: return to_view(v, p.get_config_request);
  case GET_CONFIG_REPLY: return to_view(v, p.get_config_reply);
  case SET_CONFIG: return to_view(v, p.set_config);
  case PACKET_IN: return to_view(v, p.packet_in);
  case FLOW_REMOVED: return to_view(v, p.flow_removed);
  case PORT_STATUS: return to_view(v, p.port_status);
  case PACKET_OUT: return to_view(v, p.packet_out);
  case FLOW_MOD: return to_view(v, p.flow_mod);
  case PORT_MOD: return to_view(v, p.port_mod);
  case STATS_REQUEST: return to_view(v, p.stats_request);
  case STATS_REPLY: return to_view(v, p.stats_reply);
  case BARRIER_REQUEST: return to_view(v, p.barrier_request);
  case BARRIER_REPLY: return to_view(v, p.barrier_reply);
  case QUEUE_GET_CONFIG_REQUEST: return to_view(v, p.queue_get_config_request);
  case QUEUE_GET_CONFIG_REPLY: return to_view(v, p.queue_get_config_reply);
  */
  default:
    return Errc::BAD_MESSAGE_TYPE;
  }
}

Errc
from_view(View& v, Message& m) {
  Message::Payload& p = m.payload;
  switch(m.type) {
  case HELLO: return from_view(v, p.hello);
  case ERROR: return from_view(v, p.error);
  /*
  case ECHO_REQUEST: return from_view(v, p.echo_request);
  case ECHO_REPLY: return from_view(v, p.echo_reply);
  case VENDOR: return from_view(v, p.vendor);
  case FEATURE_REQUEST: return from_view(v, p.feature_request);
  case FEATURE_REPLY: return from_view(v, p.feature_reply);
  case GET_CONFIG_REQUEST: return from_view(v, p.get_config_request);
  case GET_CONFIG_REPLY: return from_view(v, p.get_config_reply);
  case SET_CONFIG: return from_view(v, p.set_config);
  case PACKET_IN: return from_view(v, p.packet_in);
  case FLOW_REMOVED: return from_view(v, p.flow_removed);
  case PORT_STATUS: return from_view(v, p.port_status);
  case PACKET_OUT: return from_view(v, p.packet_out);
  case FLOW_MOD: return from_view(v, p.flow_mod);
  case PORT_MOD: return from_view(v, p.port_mod);
  case STATS_REQUEST: return from_view(v, p.stats_request);
  case STATS_REPLY: return from_view(v, p.stats_reply);
  case BARRIER_REQUEST: return from_view(v, p.barrier_request);
  case BARRIER_REPLY: return from_view(v, p.barrier_reply);
  case QUEUE_GET_CONFIG_REQUEST: return from_view(v, p.queue_get_config_request);
  case QUEUE_GET_CONFIG_REPLY: return from_view(v, p.queue_get_config_reply);
  */
  default:
    return Errc::BAD_MESSAGE_TYPE;
  }
}

} // namespace v1_0
} // namespace ofp
} // namespace freeflow
