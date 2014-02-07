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
EPevent::EPevent(Type t, int fd)
{
  events = t;
  data.fd = fd;
}

inline
Epoll::Epoll() :
  fd(::epoll_create(1))
{
  if(fd == -1)
    throw system_error();
}

inline
Epoll::~Epoll()
{
  if(fd > -1)
    ::close(fd);
}

inline Error 
add_reader(Epoll& ep, int fd)
{
  EPevent ev(EPevent::READ, fd);
  ep.watched.insert(std::pair<int,EPevent>(fd, ev));
  if(::epoll_ctl(ep.fd, EPOLL_CTL_ADD, fd, &ev) == -1)
    return system_error();
  return Error();
}

inline Error
add_writer(Epoll& ep, int fd)
{
  EPevent ev(EPevent::WRITE, fd);
  ep.watched.insert(std::pair<int,EPevent>(fd, ev));
  if(::epoll_ctl(ep.fd, EPOLL_CTL_ADD, fd, &ev) == -1)
    return system_error();
  return Error();
}

inline Error
del_reader(Epoll& ep, int fd)
{
  EPevent ev(EPevent::READ, fd);
  ep.watched.insert(std::pair<int,EPevent>(fd, ev));
  if(::epoll_ctl(ep.fd, EPOLL_CTL_DEL, fd, &ev) == -1)
    return system_error();
  return Error();
}

inline Error
del_writer(Epoll& ep, int fd)
{
  EPevent ev(EPevent::WRITE, fd);
  ep.watched.insert(std::pair<int,EPevent>(fd, ev));
  if(::epoll_ctl(ep.fd, EPOLL_CTL_DEL, fd, &ev) == -1)
    return system_error();
  return Error();
}

inline EPevents
poll(Epoll& ep, const MicroTime& mt)
{
  EPevents events(ep.watched.size());
  if(::epoll_wait(ep.fd, events.data(), events.size(), mt.count() * 1000) == -1)
    throw system_error();
  return events;
}

} // namespace freeflow
