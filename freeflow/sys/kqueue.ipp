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

inline
KEvent::KEvent(uintptr_t id, Type fltr, Operation flg, uint32_t fflg, 
               intptr_t d, void* ud)
{
  ident  = id;
  filter = fltr;
  flags  = flg;
  fflags = fflg;
  data   = d;
  udata  = ud;
}

inline KEvent
mk_kevent_vnode(uintptr_t id, KEvent::Operation op, KEvent::VNode flags, void* opaque)
{
  return KEvent(id, KEvent::VNODE, op, flags, 0, opaque);
}

inline KEvent
mk_kevent_proc(uintptr_t id, KEvent::Operation op, KEvent::Proc flags, void* opaque)
{
  return KEvent(id, KEvent::PROC, op, flags, 0, opaque);
}

inline KEvent
mk_kevent_time(uintptr_t id, KEvent::Operation op, KEvent::Time flags, intptr_t d, void* opaque)
{
  return KEvent(id, KEvent::TIME, op, flags, d, opaque);
}

inline KEvent
mk_kevent_read(uintptr_t id, KEvent::Operation op, void* opaque)
{
  return KEvent(id, KEvent::READ, op, 0, 0, opaque);
}

inline KEvent
mk_kevent_write(uintptr_t id, KEvent::Operation op, void* opaque)
{
  return KEvent(id, KEvent::WRITE, op, 0, 0, opaque);
}

inline
KQueue::KQueue() :
  queue(::kqueue())
{
  if(queue == -1)
    throw system_error();
}

inline
KQueue::~KQueue()
{
  if(queue > -1) {
    ::close(queue);
    queue = -1;
  }
}

inline void 
add_reader(KQueue& kq, int fd) 
{
  kq.in_events[fd] = mk_kevent_read(fd, KEvent::ADD);
  kq.active.insert(std::pair<int,uint16_t>(fd, KEvent::READ));
}

inline void 
add_writer(KQueue& kq, int fd) 
{
  kq.in_events[fd] = mk_kevent_write(fd, KEvent::ADD);
  kq.active.insert(std::pair<int,uint16_t>(fd, KEvent::WRITE));
}

inline void
add_timer(KQueue& kq, int id, const MicroTime& mt)
{
  kq.in_events[id] = mk_kevent_time(id, KEvent::ADD, KEvent::TIME_USECONDS, mt.count());
  kq.active.insert(std::pair<int,uint16_t>(id, KEvent::TIME));
}

inline void 
del_reader(KQueue& kq, int fd) 
{
  kq.in_events[fd] = mk_kevent_read(fd, KEvent::DELETE);
  kq.active.erase(std::pair<int,uint16_t>(fd, KEvent::READ));
}

inline void 
del_writer(KQueue& kq, int fd) 
{
  kq.in_events[fd] = mk_kevent_write(fd, KEvent::DELETE);
  kq.active.erase(std::pair<int,uint16_t>(fd, KEvent::WRITE));
}

inline void
del_timer(KQueue& kq, int id) 
{
  kq.in_events[id] = mk_kevent_time(id, KEvent::DELETE, KEvent::TIME_USECONDS, 0);
  kq.active.erase(std::pair<int,uint16_t>(id, KEvent::TIME));
}

inline KEvents
kevent(KQueue& kq, const MicroTime& mt)
{
  KEvents input;
  for(auto event : kq.in_events)
    input.push_back(event.second);
  kq.in_events.clear();

  timespec ts;
  KEvents output(kq.active.size());

  int N = ::kevent(kq.queue, input.data(), input.size(), 
                   output.data(), output.size(), &ts);

  output.resize(N);

  return output;
}

} // namespace freeflow
