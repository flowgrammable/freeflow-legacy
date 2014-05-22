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

#include "noflow.hpp"

/// The application factory.
static Factory factory_;

extern "C" void*
factory() { return &factory_; }

ff::Application* 
Factory::construct() { return new Noflow; }

void 
Factory::destroy(ff::Application* a) { delete a; }


/// \todo Dynamically configure the application so that it can terminate
/// on different phases.
void
Noflow::load(ff::Controller&) { stop_ = ON_BIND; }

void
Noflow::bind(ff::Switch& s) {
  if (stop_ == ON_BIND)
    s.disconnect();
}

void
Noflow::version_known(ff::Switch& s) {
  if (stop_ == ON_VERSION)
    s.disconnect();
}

// The application never operates beyond this point.
void
Noflow::features_known(ff::Switch& s) { s.disconnect(); }
