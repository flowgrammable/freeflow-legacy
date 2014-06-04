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

#ifndef FREEFLOW_LIBRARY_HPP
#define FREEFLOW_LIBRARY_HPP

#include <dlfcn.h>

#include <iostream>
#include <stdexcept>

#include <freeflow/sys/path.hpp>

namespace freeflow {

/// The Library class represents a dynmaically loaded library. The
/// library is loaded by the dynamic linker on construction: its symbols
/// are kept local, and all symbols are resolved immediately.  The
/// libary is unloaded when it is destroyed.
///
/// Note that dynamically loaded libraries are reference-counted, so
/// loading the same library twice will return equal objects. The
/// dynamic library is unloaded only when the last library object goes
/// out of scope.
///
/// The primray purpose of this class is to provide facilities for
/// implemnting plugin loaders.
///
/// \todo Make this non-copyable and non-movable.
class Library {
public:  
  Library(const Path&);
  ~Library();
  
  // Symbols
  void* symbol(const std::string&) const;
  
  template<typename T>
    T function(const std::string&) const;
    
  template<typename T>
    T& object(const std::string&) const;
  
  // Accessors
  const Path& path() const;
  const void* handle() const;
  
private:  
  Path  path_;   // Path to the loaded library
  void* handle_; // The underlying library
};

/// Equality comparison
bool operator==(const Library&, const Library&);
bool operator!=(const Library&, const Library&);

} // namespace freeflow

#include "library.ipp"

#endif
