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


#ifndef FREEFLOW_OFP_SEQUENCE_HPP
#define FREEFLOW_OFP_SEQUENCE_HPP

#include <vector>

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>

namespace freeflow {
namespace ofp {

/// The Sequence class represents a repeated collection of objects of
/// a particular type.
template<typename T>
  struct Sequence : public std::vector<T> {
    using std::vector<T>::vector;
  };

template<typename T>
  std::size_t bytes(const Sequence<T>&);

template<typename T>
  Error to_view(View&, const Sequence<T>&);

template<typename T>
  Error from_view(View&, Sequence<T>&);

} // namespace ofp
} // namespace freeflow

#endif
