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

#ifndef NOCONTROL_HANDLER_HPP
#define NOCONTROL_HANDLER_HPP

#include <utility>
#include <vector>

#include <freeflow/sys/resource.hpp>

#include <nocontrol/config.hpp>

namespace nocontrol {

// Result codes for events.
// TODO: Find a better name.
enum Result {
  CONTINUE = 0,
  STOP = -1,
  EXIT = -2
};


/// The Handler class defines the interface required by all specific 
/// handler instances. An abstract handler is associated with a 
/// resource, which provides access to an underlying socket descriptor.
class Handler {
public:
  explicit Handler(ff::Resource&);
  virtual ~Handler() { }

  // Open/close
  virtual bool open();
  virtual bool close();

  // Event handlers
  virtual Result on_read();
  virtual Result on_write();
  virtual Result on_error();
  virtual Result on_time();
  virtual Result on_signal();

  // Observers
  int fd() const;

private:
  ff::Resource& r_;
};

/// The resource handler class is an abstract handler associated with
/// a Resource. The type of that Resource is given as the template
/// argument, T. It must derive from the Resource class.
template<typename T>
  class Resource_handler : public Handler {
  public:
    Resource_handler() = delete;

    // Non-copyable
    Resource_handler(const Resource_handler&) = delete;
    Resource_handler& operator=(const Resource_handler&) = delete;

    // Non-movable
    Resource_handler(Resource_handler&&) = delete;
    Resource_handler& operator=(Resource_handler&&) = delete;

    // Resource initialization
    Resource_handler(T&&);

    template<typename... Args>
      Resource_handler(Args&&... args);

    // Obsrevers
    T&       rc();
    const T& rc() const;

  private:
    T rc_;
  };

/// The handler registry maintains the set of handlers registered
/// for the reactor loop.
///
/// \todo This could also house the wait set, the fd set that describes
/// what the handler is waiting on.
///
/// \todo The size is currently limited to fd_setsize, which limits
/// the number of connections to 1024. That may be too small.
struct Handler_registry : std::vector<Handler*> {
  Handler_registry();

  // Registration
  bool add(Handler* h);
  bool remove(Handler* h);

  int max() const;

  // Singleton access
  static Handler_registry& instance();

private:
  int active_;
  int max_;
};

} // namespace nocontrol

#include "handler.ipp"

#endif
