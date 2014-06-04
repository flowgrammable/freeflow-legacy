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

#ifndef FREEFLOW_OFPV1_0_QUEUE_HPP
#define FREEFLOW_OFPV1_0_QUEUE_HPP

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/ofp.hpp>
#include <freeflow/proto/ofp/v1.0/error.hpp>

namespace freeflow {
namespace ofp {
namespace v1_0 {

enum Property_type : Uint16 {
  PROP_MIN_RATE = 0x01,
};

/// A rate property.
struct Rate_property {
  Uint16 rate;
};

union Property_value {
  Rate_property rate;
};

/// The property class represents a property of a queue. The specific
/// kind of property is denoted by the type field.
struct Property {
  using Type = Property_type;
  using Value = Property_value;

  Type   property;
  Uint16 length;
  Value  value;
};

/// A sequence of properties.
using Property_list = Sequence<Property>;

/// Represents a queue. A queue is described by an id and a list of
/// of properties.
struct Queue {
  using Id = Uint32;

  Id            queue_id;
  Uint16        length;
  Property_list properties;
};

/// A sequence of queues.
using Queue_list = Sequence<Queue>;

// Protocol
using ofp::bytes;
using ofp::to_view;
using ofp::from_view;

// Bytes
constexpr std::size_t bytes(const Rate_property&);
std::size_t bytes(const Property_value&, Property_type);
std::size_t bytes(const Property&);
std::size_t bytes(const Queue&);

// To view
Error to_view(View&, const Rate_property&);
Error to_view(View&, const Property_value&, Property_type);
Error to_view(View&, const Property&);
Error to_view(View&, const Queue&);

// From view
Error from_view(View&, Rate_property&);
Error from_view(View&, Property_value&, Property_type);
Error from_view(View&, Property&);
Error from_view(View&, Queue&);

// Validation
constexpr bool is_valid(Property_type);

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#include "queue.ipp"

#endif
