// Copyright (c) 2013-2014 Flowgrammable, LLC.
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

/// Load an application from the given library, returning true when the
/// application loads successfully. If the application does not load,
/// it is not installed.
///
/// \todo Suppo
// inline void
// Controller::load(Application_library& lib) {
//   Application* a = lib.create();
//   if (not a->load(*this)) {
//     delete a;
//     return;
//   } else {
//     apps_.push_back(a);
//   }
// }

/// Unload the application of the given type.
// inline void
// Controller::unload(Application_library& lib, Application* app) {
//   assert(apps_.count(app));
//   app->unload(*this);
//   apps_.erase(app);
//   lib.destroy(app);
// }

} // namespace freeflow

