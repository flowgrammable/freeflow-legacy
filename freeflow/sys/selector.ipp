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
Selector::Selector(int n, Select_set& ss)
  : max_(n), read_(&ss.read.fds), write_(&ss.write.fds), except_(&ss.except.fds) { }

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

inline int
Selector::operator()() {
  int r = ::pselect(max_, read_, write_, except_, nullptr, nullptr);
  return impl::select_result(r);
}

inline int
Selector::operator()(Microseconds us) {
  // FIXME: Make an API that translates time units to timespecs.
  timespec ts;
  ts.tv_sec = std::chrono::duration_cast<Seconds>(us).count();
  ts.tv_nsec = 1000 * (us.count() % 1000000);
  
  int r = ::pselect(max_, read_, write_, except_, &ts, nullptr);
  return impl::select_result(r);
}

} // namespace freeflow
