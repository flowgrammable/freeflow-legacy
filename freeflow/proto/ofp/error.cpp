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

#include "error.hpp"

namespace freeflow {
namespace ofp {

namespace {
struct Errors : Error_category {
  const char* name() const noexcept override;
  
  std::string message(int ec) const override;
};

const char* 
Errors::name() const noexcept { return "openflow"; }

std::string 
Errors::message(int ec) const {
  switch (static_cast<errc>(ec)) {
  case errc::header_overflow: return "header overflow";
  case errc::payload_overflow: return "payload_overflow";
  case errc::sequence_overflow: return "sequence_overflow";

  case errc::bad_version: return "bad version";
  case errc::bad_header_length: return "bad header length";
  case errc::bad_header_type: return "bad header type";
  }
  assert(false); // Unreachable
  return "";
}
} // namesapce

const Error_category&
ofp_error_category() {
  static Errors errs_;
  return errs_;
}

} // namespace ofp
} // namespace freeflow

