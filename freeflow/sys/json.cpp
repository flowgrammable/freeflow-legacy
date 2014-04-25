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

#include <iostream>

#include <freeflow/sys/json.hpp>
#include <freeflow/sys/file.hpp>

namespace freeflow {
namespace json {

namespace {
Value
parse(const char* first, const char* last) {
  return {};
}
}

Value
parse(File& f) {
  // Read the entire file into memory and then parse it.
  // FIXME: If the parser were stream-based, we could a) avoid a single
  // alloc/read, and b) make this work for resources, not just files.
  std::size_t n = f.size();
  char* buf = new char[n];
  f.read(buf, n);
  Value v = parse(buf, buf + n);
  delete buf;
  return v;
}

Value
parse(const std::string& str) {
  return parse(str.c_str(), str.c_str() + str.size());
}


} // namespace json
} // namespace freeflow
