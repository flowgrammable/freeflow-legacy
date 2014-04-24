// Copyright (c) 2013-2014 Flowgrammable, LLC.
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

#include <freeflow/sys/json.hpp>
#include <freeflow/sys/buffer.hpp>

namespace freeflow {
namespace json {

namespace {
Value
parse(const char* first, const char* last) {
  return {};
}
}

/// Parse a 
Value
parse(Resource& rc) {
  // Read the resource into a string and then parse the buffer.
  std::string str;
  return parse(str);
}

Value
parse(const std::string& str) {
  return parse(str.c_str(), str.c_str() + str.size());
}


} // namespace json
} // namespace freeflow
