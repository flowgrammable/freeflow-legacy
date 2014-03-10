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

#ifndef FREEFLOW_OFP_ERROR_HPP
#define FREEFLOW_OFP_ERROR_HPP

#include <freeflow/sys/error.hpp>

namespace freeflow {
namespace ofp {

// -------------------------------------------------------------------------- //
// Common errors

/// Extends the Error class with new codes specific to the OpenFlow
/// protocol.
struct Error : freeflow::Error {
  /// Reading or writing the header would overflow the buffer.
  static constexpr Code HEADER_OVERFLOW  = 100;
  
  /// Reading or writing the payload would overflow the buffer. 
  static constexpr Code PAYLOAD_OVERFLOW  = 101;

  /// The protocol version is unsupported.
  static constexpr Code BAD_VERSION       = 102;

  /// The header length is less than the size of the header.
  static constexpr Code BAD_HEADER_LENGTH = 104;

  using freeflow::Error::Error;
};


} // namespace ofp
} // freeflow

#endif
