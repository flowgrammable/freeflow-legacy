} // namespace freeflow
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

/// Returns the number of bytes required by the sequence.
template<typename T>
  inline std::size_t 
  bytes(const Sequence<T>& s) {
    std::size_t n = 0;
    for (const T& x : s)
      n += bytes(x);
    return n;
  }

/// Write a sequence of elements to a view.
template<typename T>
  Error 
  to_view(View& v, const Sequence<T>& s) {
    if (v.remaining() < bytes(s))
      return SEQUENCE_OVERFLOW;
    for (const auto& x : s)
      to_buffer(v, x);
    return {};
  }

/// Read a sequence of elements from a view. This is done by reading 
/// and constructing objects until no more can be constructed. 
///
/// Note that elements for which bytes() is not a constant expression
/// have indeterminate size until they are read. Because of this, the
/// algorithm must check each construction.
template<typename T>
  Error
  from_view(View& v, Sequence<T>& s) {
    while (v.remaining()) {
      T t;
      if (Trap t = from_buffer(v, t))
        return err;
      s.push_back(std::move(t));
    }
    return {};
  }

} // namespace ofp
} // namespace freeflow