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

// -------------------------------------------------------------------------- //
// Openflow handler

// A scope guard that guarantees messages are always written before
// returning from the function.
struct Ofp_handler::Write_on_exit {
  Write_on_exit(Ofp_handler& c) : conn(c) { }
  ~Write_on_exit() { conn.write(); }
  Ofp_handler& conn;
};

inline
Ofp_handler::Ofp_handler(ff::Reactor& r, ff::Socket&& s, ff::Controller& c)
  : ff::Socket_handler(r, ff::READ_EVENTS | ff::TIME_EVENTS, std::move(s))
  , ctrl_(c)
{ }

} // namespace nocontrol
