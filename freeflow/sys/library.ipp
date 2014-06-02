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
Library::Library(const Path& p) 
  : path_(p), handle_(dlopen(p.c_str(), RTLD_LOCAL | RTLD_NOW))
{
  if (not handle_) 
    throw std::runtime_error(dlerror());
}

/// Destroy the object, unloading the underlying library.
inline
Library::~Library() { dlclose(handle_); }

/// Returns an opaque pointer to the symbol in the loaded library. If
/// the symbol cannot be resolved, an exception is thrown.
inline void*
Library::symbol(const std::string& s) const {
  // NOTE: This does not use dlsym in the way recommended by Linux
  // man pages. However, this is not intended for general usage, but
  // very constrained usage, so it should be okay in this context.
  void* p= dlsym(handle_, s.c_str());
  if (not p)
    throw std::runtime_error(dlerror());
  return p;
}

/// Returns the function from the loaded library. 
///
/// \todo This function may rely on undefined behavior to reinterpret
/// a void pointer as a function pointer. Find a way to guarantee that
/// behavior is defined.
template<typename T>
  inline T
  Library::function(const std::string& s) const { return (T)symbol(s); }
 
/// Returns the object symbol from the loaded library.
template<typename T>
  inline T&
  Library::object(const std::string& s) const { return (T&)symbol(s); }

/// Return the path from which the library was loaded.
inline const Path& 
Library::path() const { return path_; }

/// Return the library's handle
inline const void* 
Library::handle() const { return handle_; }

/// Returns true when two libraries refer to the same dynamically loaded
/// library. This is the case when the two libraries have the same handle.
inline bool 
operator==(const Library& a, const Library& b) {
  return a.handle() == b.handle();
}

inline bool 
operator!=(const Library& a, const Library& b) {
  return !(a == b);
}


} // namespace freeflow
