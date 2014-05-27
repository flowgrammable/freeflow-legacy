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

namespace nocontrol {

inline
Switch_factory::Switch_factory(ff::Controller& c)
  : ctrl(c) { }

/// Create a new connection for the socket.
inline Connection* 
Switch_factory::operator()(ff::Reactor& r, ff::Socket&& s) {
  return new Connection(r, ctrl, std::move(s));
}

} // namesapce nocontrol
