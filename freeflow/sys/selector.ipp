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
  : max_(n), read_(&ss.read.fds), write_(&ss.write.fds), error_(&ss.error.fds) { }

// TODO: Allow selection over some amount of time.
inline int
Selector::operator()() {
  int r = ::pselect(max_, read_, write_, error_, nullptr, nullptr);
  if(r == -1) {
    if (errno != EINTR)
      throw system_error();
    else
      return 0;
  }
  return r;
}

/*
// TODO: pselect will update the timespec based on the amount of time
// elapsed. It would be nice to return this to the user, but I'm not
// sure what the right interface should be. Maybe this should
// return pair<int, Microseconds>.
inline int
Selector::operator()(Microseconds us) {
  // TODO: Factor this into the time module.
  timespec ts;
  ts.tv_sec = std::chrono::duration_cast<Seconds>(us).count();
  ts.tv_nsec = 1000 * (us.count() % 1000000);

  int m = std::max(readers.max, writers.max) + 1;
  int r = ::pselect(m, &readers.fds, &writers.fds, nullptr, &ts, nullptr);
  if(r == -1 and errno != EINTR)
    throw system_error();

  return r;
}
*/

} // namespace freeflow
