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
  inline void 
  Message_queue::put_message(const H& h, const P& p) {
    emplace(bytes(h) + bytes(p));
    
    Buffer& buf = back();
    Byte* p1 = buf.data();
    Byte* p2 = buf.data() + bytes(h);
    Byte* p3 = p2 + bytes(p);

    View v1(buf, p1, p2);
    to_view(v1, h);

    View v2(buf, p2, p3);
    to_view(v2, p);
  }

/// Read a header from the front of the queue, but do not
/// remove the buffer.
template<typename H>
  inline void 
  Message_queue::peek_header(H& h) {
    Buffer& buf = front();
    Byte* p1 = buf.data();
    Byte* p2 = buf.data() + bytes(h);

    View v(buf, p1, p2);
    from_view(v, h);
  }

template<typename H, typename P>
  inline void 
  Message_queue::get_payload(const H& h, P& p) {
    Buffer& buf = front();
    Byte* p1 = buf.data() + bytes(h);
    Byte* p2 = buf.data() + buf.size() - bytes(h);

    View v(buf, p1, p2);
    from_view(v, p);
    pop();
  }


template<typename H, typename P>
  inline void 
  Protocol::put_message(const H& h, const P& p) { write.put_message(h, p); }

template<typename H>
  inline void 
  Protocol::peek_header(H& h) { read.peek_header(h); }

template<typename H, typename P>
  inline void 
  Protocol::get_payload(const H& h, P& p) { read.get_payload(h, p); }


} // ofp
} // freeflow
