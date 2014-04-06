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
#include <freeflow/sys/selector.hpp>

#include <nocontrol/config.hpp>

namespace nocontrol {

class Reactor;

/// The Handler class defines the interface required by all specific 
/// handler instances. An abstract handler is associated with a 
/// resource, which provides access to an underlying socket descriptor.
class Handler {
public:
  explicit Handler(ff::Resource&);
  virtual ~Handler() { }

  // Events
  virtual bool on_open(Reactor&);
  virtual bool on_close(Reactor&);
  virtual bool on_read(Reactor&);
  virtual bool on_write(Reactor&);
  virtual bool on_error(Reactor&);
  virtual bool on_time(Reactor&, int);
  virtual bool on_signal(Reactor&);

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

    // Observers
    T&       rc();
    const T& rc() const;

  private:
    T rc_;
  };

/// The handler registry maintains the set of handlers registered
/// for the reactor loop. The set maintained by the registery is sparse;
/// iteration over the set may include traversal over null pointers.
///
/// \todo The size is currently limited to FD_SETSIZE, which limits
/// the number of connections to 1024. That may be too small.
///
/// \todo Consider a more efficient structure for representing the
/// set. It would be ideal if iteration required as many increments
/// as elements in the set. Note that std::set may be too slow.
///
/// \todo Find a way to break the coupling with the reactor.
struct Handler_registry : std::vector<Handler*> {
  Handler_registry();

  // Registration
  bool add(Reactor& r, Handler* h);
  bool remove(Reactor& r, Handler* h);

  int max() const;

  // Wait set
  const ff::Select_set& wait() const;

private:
  int            active_;
  int            max_;
  ff::Select_set wait_;
};

} // namespace nocontrol

#include "handler.ipp"

#endif
