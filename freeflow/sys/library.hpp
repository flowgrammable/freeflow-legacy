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

#include <iostream>
#include <dlfcn.h>
#include <string>

namespace freeflow {

/*
struct Plugin {
	void* handle; // handle for the dynamically loaded library
};

struct Plugin_loader {
	void open(std::string lib_name);
	void close(Plugin lib);
};
*/
struct Library {
	std::string lib_name;
	void Library();
	void ~Library();
	void* handle;
};

void Library::open() {
	handle = dlopen("./apps/noflow/libnoflow.so", RTLD_LAZY);
  if (!handle) {
    std::cerr << "Cannot open library: " << dlerror() << '\n';
  }
}
#include "library.ipp"

} // namespace freeflow
#endif
