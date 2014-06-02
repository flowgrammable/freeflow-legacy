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
namespace cli {


inline void
Add::operator()(const Arguments& args) const {
  std::cout << "Ran add" << "\n";
}

inline void
Remove::operator()(const Arguments& args) const {
  std::cout << "Ran remove" << "\n";
}

// no-op
inline void
Help::operator()(const Arguments& args) const { }



} // namespace cli
} // namespace freeflow