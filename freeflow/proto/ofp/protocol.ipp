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

namespace freeflow {
namespace ofp {

/// Move the uninterpted buffer to the back of the queue.
inline void 
Message_queue::put_buffer(Buffer&& buf) {
  push(std::move(buf));
}

/// Remove the front message, moving it to buf.
inline void 
Message_queue::get_buffer(Buffer& buf) {
  buf = std::move(front());
  pop();
}

/// Put a message in the back of the queue where the message is 
/// comprised of a header and its payload.
template<typename H, typename P>
  inline Error 
  Message_queue::put_message(const H& h, const P& p) {
    emplace(bytes(h) + bytes(p));
    
    Buffer& buf = back();
    Byte* p1 = buf.data();
    Byte* p2 = buf.data() + bytes(h);
    Byte* p3 = p2 + bytes(p);

    View v1(buf, p1, p2);
    if (Trap err = to_view(v1, h)) {
      pop();
      return err.error();
    }

    View v2(buf, p2, p3);
    if (Trap err = to_view(v2, p)) {
      pop();
      return err.error();
    }
    return {};
  }

/// Read a header from the front of the queue, but do not
/// remove the buffer.
template<typename H>
  inline Error 
  Message_queue::peek_header(H& h) {
    Buffer& buf = front();
    Byte* p1 = buf.data();
    Byte* p2 = buf.data() + bytes(h);

    View v(buf, p1, p2);
    return from_view(v, h);
  }

template<typename H, typename P>
  inline Error 
  Message_queue::get_payload(const H& h, P& p) {
    Buffer& buf = front();
    Byte* p1 = buf.data() + bytes(h);
    Byte* p2 = buf.data() + buf.size() - bytes(h);

    View v(buf, p1, p2);
    Error err = from_view(v, p);
    pop();
    return err;
  }

// -------------------------------------------------------------------------- //
// Protocol

inline
Protocol::Protocol(Controller* c, Handler* h)
  : handler_(h), proto_(nullptr), config_(), state_(CLOSED)
  , ctrl_(c), switch_(nullptr)
{ }

/// Create a message and put it into the message queue.
template<typename P>
  inline Error 
  Protocol::put_message(const P& p) {
    Header h {
      switch_->protocol_version(), P::Kind, Uint16(bytes(h) + bytes(p)), xid()
    };
    return write.put_message(h, p); 
  }

/// Read the header of the message at the top of the queue.
template<typename H>
  inline Error 
  Protocol::peek_header(H& h) { return read.peek_header(h); }

/// Read the payload into the message.
template<typename H, typename P>
  inline Error 
  Protocol::get_payload(const H& h, P& p) {
    assert(h.type == P::Kind);
    return read.get_payload(h, p); 
  }

/// Generate the next xid.
///
/// \todo There must be a better strategy than just this. The only real
/// requirement is that no two requests have the same xid. Otherwise, it
/// may not matter.
inline Uint32
Protocol::xid() { return xid_++; }

} // ofp
} // freeflow
