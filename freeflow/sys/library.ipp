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

/// Initalize an object by loading the library indicated by
/// the specified path. If the library cannot be opened,
/// an exception is thrown.
///
/// The library is loaded so that its symbols are local,
/// meaning that no other libraries can be resolved against
/// them.
inline
Library::Library(const std::string& p) 
  : path_(p), handle_(dlopen(p.c_str(), RTLD_LOCAL | RTLD_NOW))
{
  if (not handle_) 
    throw std::runtime_error(dlerror());
}

/// Destroy the object, unloading the underlying library.
inline
Library::~Library() { 
  dlclose(handle_);
}

/// Retrieve the symbol from the loaded library
inline
void*
Library::symbol(std::string symbol) const {
  char* error;
  dlerror(); // clear existing error
  void* sym = dlsym(handle_, symbol.c_str());
  if ((error = dlerror()) != NULL) {
    throw std::runtime_error(error);
  }
  return sym;
}

inline
const Path& 
Library::path() const {
  return path_;
}

} // namespace freeflow
