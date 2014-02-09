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

#ifndef FREEFLOW_KQUEUE_HPP
#define FREEFLOW_KQUEUE_HPP

#include <unistd.h>
#include <sys/event.h>
#include <sys/types.h>

#include <map>
#include <set>
#include <vector>

#include "freeflow/sys/error.hpp"
#include "freeflow/sys/time.hpp"

namespace freeflow {

struct KEvent : kevent
{

  enum Operation : uint16_t {
    ADD    = EV_ADD, 
    DELETE = EV_DELETE,
  };

  enum Type : int16_t {
    READ  = EVFILT_READ, 
    WRITE = EVFILT_WRITE, 
    TIME  = EVFILT_TIMER,
    VNODE = EVFILT_VNODE,
    PROC  = EVFILT_PROC,
  };

  enum VNode : uint32_t {
    VNODE_DELETE = NOTE_DELETE,
    VNODE_WRITE  = NOTE_WRITE,
    VNODE_EXTEND = NOTE_EXTEND,
    VNODE_ATTRIB = NOTE_ATTRIB,
    VNODE_LINK   = NOTE_LINK,
    VNODE_RENAME = NOTE_RENAME,
    VNODE_REVOKE = NOTE_REVOKE,
  };

  enum Proc : uint32_t {
    PROC_EXIT       = NOTE_EXIT,
    PROC_EXITSTATUS = NOTE_EXITSTATUS,
    PROC_FORK       = NOTE_FORK,
    PROC_EXEC       = NOTE_EXEC,
    PROC_SIGNAL     = NOTE_SIGNAL,
    PROC_REAP       = NOTE_REAP,
  };

  enum Time : uint32_t {
    TIME_SECONDS    = NOTE_SECONDS,
    TIME_USECONDS   = NOTE_USECONDS,
    TIME_NSECONDS   = NOTE_NSECONDS,
    TIME_ABSOLUTE   = NOTE_ABSOLUTE,
    TIME_CRITICAL   = NOTE_CRITICAL,
    TIME_BACKGROUND = NOTE_BACKGROUND,
  };

  KEvent() = default;
  KEvent(const KEvent&) = default;
  KEvent(uintptr_t id, Type fltr, Operation flg, uint32_t fflg, 
         intptr_t d, void* ud);

};

KEvent
mk_kevent_vnode(uintptr_t id, KEvent::Operation op, KEvent::VNode flags, 
                void* opaque = nullptr);

KEvent
mk_kevent_proc(uintptr_t id, KEvent::Operation op, KEvent::Proc flags, 
               void* opaque = nullptr);

KEvent
mk_kevent_time(uintptr_t id, KEvent::Operation op, KEvent::Time flags, 
               intptr_t d, void* opaque = nullptr);

KEvent
mk_kevent_read(uintptr_t id, KEvent::Operation op, void* opaque = nullptr);

KEvent
mk_kevent_write(uintptr_t id, KEvent::Operation op, void* opaque = nullptr);


using KEvents = std::vector<KEvent>;

struct KQueue
{
  KQueue();
  ~KQueue();

  std::map<int,KEvent> in_events;
  int queue;
  std::set<std::pair<int,uint16_t>> active;
};

void add_reader(KQueue& kq, int fd);
void add_writer(KQueue& kq, int fd);
void add_timer(KQueue& kq, int fd, const MicroTime& mt);
void del_reader(KQueue& kq, int fd);
void del_writer(KQueue& kq, int fd);
void del_timer(KQueue& kq, int fd);

KEvents kevent(KQueue& kq, const MicroTime& mt);

} // namespace freeflow

#include "kqueue.ipp"

#endif
