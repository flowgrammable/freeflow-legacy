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

#include <iostream>

namespace freeflow {

inline
Selector::Selector(int n, Select_set& ss)
  : max_(n)
  , read_(&ss.read.fds)
  , write_(&ss.write.fds)
  , except_(&ss.except.fds) 
{ }

namespace impl {
// Check the result of calling pselect. If a non-interruption error
// occurred, throw a system error.
inline int
select_result(int r) {
  if (r == -1) {
    if (errno != EINTR)
      throw system_error();
    else
      return 0;
  }
  return r;
}
} // namespace impl


/// If the pselect call is interruped, it will return 0. This allows
/// a program to process events as they occur instead of allowing them
/// to be pending until an I/O event occurs.
inline int
Selector::select(timespec* ts, const sigset_t* m) {
  System_result r = ::pselect(max_, read_, write_, except_, ts, m);
  if (r.failed())
    throw std::runtime_error(::strerror(errno));
  else if (r.interrupted())
    return 0;
  else
    return r.value();
}

inline int
Selector::operator()() {
  return select(nullptr, nullptr);
}

inline int
Selector::operator()(const Signal_set& m) {
  return select(nullptr, m);
}

inline int
Selector::operator()(Microseconds us) {
  // FIXME: Make an API that translates time units to timespecs.
  timespec ts;
  ts.tv_sec = std::chrono::duration_cast<Seconds>(us).count();
  ts.tv_nsec = 1000 * (us.count() % 1000000);
  return select(&ts, nullptr);
}

inline int
Selector::operator()(Microseconds us, const Signal_set& m) {
  // FIXME: Make an API that translates time units to timespecs.
  timespec ts;
  ts.tv_sec = std::chrono::duration_cast<Seconds>(us).count();
  ts.tv_nsec = 1000 * (us.count() % 1000000);
  return select(&ts, m);
}

} // namespace freeflow
