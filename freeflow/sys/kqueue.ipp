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
add_reader(KQueue& kq, int fd) {
  struct kevent ev;
  EV_SET(&ev, fd, EVFILT_READ, EV_ADD, 0, 0, 0);
  kq.in_events[fd] = ev;
}

inline void 
add_writer(KQueue& kq, int fd) {
  struct kevent ev;
  EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD, 0, 0, 0);
  kq.in_events[fd] = ev;
}

inline void 
del_reader(KQueue& kq, int fd) {
  struct kevent ev;
  EV_SET(&ev, fd, EVFILT_READ, EV_DELETE, 0, 0, 0);
  kq.in_events[fd] = ev;
}

inline void 
del_writer(KQueue& kq, int fd) {
  struct kevent ev;
  EV_SET(&ev, fd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
  kq.in_events[fd] = ev;
}

inline int
kevent(KQueue& kq, const MicroTime& mt)
{
  std::vector<struct kevent> input;
  for(auto event : kq.in_events)
    input.push_back(event.second);
  kq.in_events.clear();

  timespec ts;
  int N = 10;
  struct kevent output[N];

  ::kevent(kq.queue, input.data(), input.size(), output, N, &ts);

  return 0;
}

} // namespace freeflow
