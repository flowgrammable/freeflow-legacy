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

// -------------------------------------------------------------------------- //
// Process

inline
Process::Process(Application* a, Application_library* l)
  : app(a), lib(l) { }


// -------------------------------------------------------------------------- //
// Controller

/// Add a new handler that accepts connections from the specified address,
/// over the tansport protocol. The returned event handler is managed by
/// the controller object.
///
/// \todo Provide an additional argument for the backlog.
template<typename T>
  inline T*
  Controller::add_listener(const Address& a, Socket::Transport t) {
    T* h = new T(react_, *this);
    h->listen(a, t);
    return h;
  }

/// Remove the listener, closing the socket on which it accepts
/// connections. The listener is destroyed after removal.
template<typename T>
  inline void
  Controller::remove_listener(T* h) { 
    react_.remove_handler(h); 
    delete h;
  }


/// Returns true if the library is already loaded.
inline bool
Controller::is_loaded(const std::string& name) {
  return libs_.count(name) != 0;
}

/// Load an application library, returing it.
///
/// \todo Do we really want a name?
inline Application_library*
Controller::load(const std::string& name) {
  auto iter = libs_.find(name);
  if (iter != libs_.end()) {
    return iter->second;
  } else {
    Application_library* lib = new Application_library(name);
    libs_.insert({name, lib});
    return lib;
  }
}

/// Unload the application library. Unloading a library that is not
/// loaded has no effect.
inline void
Controller::unload(const std::string& name) {
  libs_.erase(name);
}

/// Start a new instance of the application with the given name. If
/// the application library is not loaded, an attempt will be made to
/// load the library.
/// 
/// \todo This should really create a process instead of an application.
///
/// \todo Provide arguments to the application.
inline Process*
Controller::start(const std::string& name) {
  // Create the application.
  // FIXME: We should be passing arguments to create.
  Application_library* lib = load(name);
  try {
    // FIXME: I don't think create returns nullptr.
    Application* app = lib->create(*this);
    if (not app) 
      return nullptr;

    // Create the process record
    auto iter = procs_.emplace(procs_.end(), app, lib);
    iter->pos = iter;

    // Perform startup
    app->start();
    return &*iter;
  } catch(...) {
    std::cerr << "error: could not load library\n";
    throw;
    // return nullptr;
  }
}

/// Terminate the process, removing it from the process list and
/// reclaiming any resources allocated to the application.
inline void
Controller::stop(Process* proc) {
  Application* app = proc->app;
  Application_library* lib = proc->lib;
  
  // Release resources.
  app->stop();
  lib->destroy(app);

  // Remove the process.
  procs_.erase(proc->pos);
}

// Process events by running the controller's event loop.
inline void
Controller::run() { react_.run(); }

} // namespace freeflow

