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
namespace ofp {
namespace v1_0 {

constexpr std::size_t 
bytes(const Rate_property&) { return 8; }

/// Returns the number of bytes in the property value. Behavior is 
/// defined only when t is valid.
inline std::size_t 
bytes(const Property_value& p, Property_type t) {
  return bytes(p.rate);
}

inline std::size_t 
bytes(const Property& m) { return 8 + bytes(m.value, m.property); }

inline std::size_t 
bytes(const Queue& m) { return 8 + bytes(m.properties); }

// Validation

constexpr bool
is_valid(Property_type t) { return t == 0x01; }

} // namespace v1_0
} // namespace ofp
} // namespace freeflow
