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

#include <stdexcept>

#include <freeflow/sys/path.hpp>

namespace freeflow {

/// The Library class...
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
  
private:  
  Path  path_;   // Path to the loaded library
  void* handle_; // The underlying library
};

} // namespace freeflow

#include "library.ipp"

#endif
