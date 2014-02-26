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

#include <string>

namespace freeflow {

/// A Path is a sequence of characters that designates a file object.
///
/// @todo This should be an actual class with a legitimate interface.
/// Or, we could rely on the C++ File System TS whenever it's implemented
/// standardized.
using Path = std::string;

} // namespace freeflow
