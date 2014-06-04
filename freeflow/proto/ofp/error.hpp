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

enum class errc {
  header_overflow,   // The header size exceeds available data
  payload_overflow,  // The payload size exceeds available data
  sequence_overflow, // A sequencesize exceeds available data
  
  bad_version,       // A header version is incorrect
  bad_header_length, // The header length is incorrect
  bad_header_type,   // The header type is incorrect
};

const Error_category& ofp_error_category();
Error_code make_error_code(errc);

} // namespace ofp
} // freeflow

#include <freeflow/proto/ofp/error.ipp>

#endif
