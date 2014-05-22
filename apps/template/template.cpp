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

#include <iostream>

#include "template.hpp"

// -------------------------------------------------------------------------- //
// Factory


/// The factory_ instance is a static global variable.
static Factory factory_;

/// The factory() function is the library hook that returns the
/// factory instance to the application loader.
extern "C" void*
factory() { return &factory_; }

/// The construct() function is responsible for allocating new instances 
/// of the application. 
///
/// Note that this does not necessarily need to dynamically allocate a
/// new object, it could return a singleton value instead.
ff::Application* 
Factory::construct() { return new Template(); }

/// The destroy() function is responsible for disposing of application
/// objects.
///
/// Note that this does not necessarily need to delete the pointed-at
/// objet. If, for example, the application is a singletone, this function
/// may do nothing.
void 
Factory::destroy(ff::Application* a) { delete a; }


// -------------------------------------------------------------------------- //
// Application

/// The load() function is called by the controller after the the
/// application has been constructed. This operation allows the application
/// to initialize resources related to the controller instance (unlike
/// the constructor, which does not have access to a controller instance).
void
Template::load(ff::Controller& ctrl) { 
  std::cout << "Application 'Template' loaded to the controller\n";
}

/// The unload() function is calle by the controller just before the
/// application is destroyed. This provides a context for the application
/// to release resources acquired during in during loading.
void
Template::unload(ff::Controller& ctrl) { 
  std::cout << "Application 'Template' unloaded from the controller\n";
}

