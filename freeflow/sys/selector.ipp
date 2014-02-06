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
Selector::Selector()
{
  FD_ZERO(&readset);
  FD_ZERO(&writeset);
  read_fds.insert(-1);
  write_fds.insert(-1);
}

inline int
max(const Selector& s)
{
  int max_read_fd = *s.read_fds.rbegin();
  int max_write_fd = *s.write_fds.rbegin();
  return std::max(max_read_fd, max_write_fd) + 1;
}

inline void
add_reader(Selector& s, int fd)
{
  FD_SET(fd, &s.readset);
  s.read_fds.insert(fd);
}

inline void
add_writer(Selector& s, int fd)
{
  FD_SET(fd, &s.writeset);
  s.write_fds.insert(fd);
}

inline void
del_reader(Selector& s, int fd)
{
  FD_CLR(fd, &s.readset);
  s.read_fds.erase(fd);
}

inline void
del_writer(Selector& s, int fd)
{
  FD_CLR(fd, &s.writeset);
  s.write_fds.erase(fd);
}

inline bool
is_readable(const Selector& s, int fd)
{
  return FD_ISSET(fd, &s.readset);
}

inline bool
is_writable(const Selector& s, int fd)
{
  return FD_ISSET(fd, &s.writeset);
}

inline int
select(Selector& s, timeval* tv)
{
  int result;
  result = ::select(max(s), &s.readset, &s.writeset, nullptr, tv);
  if(result == -1)
    throw Error(Error::SYSTEM_ERROR, errno);
  return result;
}

} // namespace freeflow
