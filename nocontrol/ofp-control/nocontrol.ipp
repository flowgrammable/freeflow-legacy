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
// Nocontrol handler

/// Initialize the event handler.
///
/// \todo Pre-allocate 1 page worth memory.
inline
Nocontrol_handler::Nocontrol_handler(ff::Reactor& r, 
                                     ff::Socket&& s, 
                                     ff::Controller& c)
  : ff::Socket_handler(r, ff::READ_EVENTS, std::move(s)), ctrl_(c), buf_(4096)
{ }

// -------------------------------------------------------------------------- //
// Nocontrol factory

inline
Nocontrol_factory::Nocontrol_factory(ff::Controller& c)
  : ctrl_(c) { }

inline Nocontrol_handler*
Nocontrol_factory::operator()(ff::Reactor& r, ff::Socket&& s) const {
  return new Nocontrol_handler(r, std:: move(s), ctrl_);
}


} // namesapce nocontrol
