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

/// The create() function is responsible for allocating new instances 
/// of the application. 
///
/// Note that this does not necessarily need to dynamically allocate a
/// new object, it could return a singleton value instead.
ff::Application* 
Factory::create(ff::Controller& c) { return new Template(c); }

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

/// The constructor function is called by the controller to start the
/// applicaiton. 
Template::Template(ff::Controller& c) 
  : Application(c) 
{
  std::cout << "* application 'template' loaded\n";
}

/// The unload() function is calle by the controller just before the
/// application is destroyed. This provides a context for the application
/// to release resources acquired during in during loading.
Template::~Template() { 
  std::cout << "* application 'template' unloaded\n";
}

