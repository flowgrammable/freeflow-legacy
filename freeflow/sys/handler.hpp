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

#ifndef FREEFLOW_HANDLER_HPP
#define FREEFLOW_HANDLER_HPP

#include <utility>
#include <vector>

#include <freeflow/sys/resource.hpp>
#include <freeflow/sys/selector.hpp>

namespace freeflow {

class File;
class Socket;
class Reactor;

/// The Event_mask type describes the kinds of events that a handler
/// can subscribe to.
using Event_mask = unsigned;
static constexpr Event_mask NO_EVENTS     = 0u;
static constexpr Event_mask READ_EVENTS   = 1u << 0;
static constexpr Event_mask WRITE_EVENTS  = 1u << 1;
static constexpr Event_mask EXCEPT_EVENTS = 1u << 2;
static constexpr Event_mask TIME_EVENTS   = 1u << 3;
static constexpr Event_mask SIGNAL_EVENTS = 1u << 4;
static constexpr Event_mask ALL_EVENTS    = -1u;

/// Handler flags are managed by the reactor for the purpose of internal
/// book keeping.
using Handler_flags = unsigned;
static constexpr Handler_flags HANDLER_IS_OWNED     = 1u << 0;
static constexpr Handler_flags HANDLER_IS_ACCEPTOR  = 1u << 1;
static constexpr Handler_flags HANDLER_IS_CONNECTOR = 1u << 2;


/// The Event_handler class defines the interface required by all specific 
/// handler instances. Each event handler is associated with a resource
/// which may be the source of events.
///
/// An event handler also keeps tracks of the events to which it is
/// subscribed.
class Event_handler {
public:
  explicit Event_handler(Reactor&, int, Event_mask);
  virtual ~Event_handler() { }

  // Events
  virtual bool on_open();
  virtual bool on_close();
  virtual bool on_read();
  virtual bool on_write();
  virtual bool on_except();
  virtual bool on_time(int);
  virtual bool on_signal(int);

  // Observers
  int fd() const;
  Reactor& reactor();

  // Event management
  Event_mask events() const;
  bool is_subscribed(Event_mask) const;
  void subscribe(Event_mask);
  void unsubscribe(Event_mask);

  // Handler management
  Handler_flags flags() const;
  bool has_flags(Handler_flags) const;
  void set_flags(Handler_flags);
  void clear_flags(Handler_flags);

private:
  Reactor&      react_;  // The attached reactor
  Handler_flags flags_;  // Internal bookkeeping
  Event_mask    events_; // Subscribed events
  int           fd_;     // Underlying file descriptor
};


// A helper class that ensures proper initialization order for
// basic event handlers.
template<typename T>
  struct Resource_wrapper {
    template<typename... Args>
      explicit Resource_wrapper(Args&&...);

    T rc_;
  };

/// The resource handler class is an abstract handler associated with
/// a Resource. The type of that Resource is given as the template
/// argument, T. It must derive from the Resource class.
template<typename T>
  class Basic_event_handler 
    : protected Resource_wrapper<T>
    , public Event_handler 
  {
  public:
    // Non-default constructible.
    Basic_event_handler() = delete;

    // Non-movable
    Basic_event_handler(Basic_event_handler&&) = delete;
    Basic_event_handler& operator=(Basic_event_handler&&) = delete;

    // Non-copyable
    Basic_event_handler(const Basic_event_handler&) = delete;
    Basic_event_handler& operator=(const Basic_event_handler&) = delete;

    // Resource initialization
    template<typename... Args>
      Basic_event_handler(Reactor&, Event_mask, Args&&... args);

    // Accessors
    T&       rc();
    const T& rc() const;
  };

/// A resource handler is a handler for an attached resource. This
/// is useful for attaching a resource to an existing file descriptor.
using Resource_handler = Basic_event_handler<Resource>;

/// A Socket_handler is an event handler for a socket.
using Socket_handler = Basic_event_handler<Socket>;

/// A File_handler is an event handler for a file.
using File_handler = Basic_event_handler<File>;


/// The handler registry maintains the set of handlers registered
/// for the reactor loop. The set maintained by the registry is sparse;
/// iteration over the set may include traversal over null pointers.
///
/// \todo The size is currently limited to FD_SETSIZE, which limits
/// the number of connections to 1024. That may be too small.
///
/// \todo Consider a more efficient structure for representing the
/// set. It would be ideal if iteration required as many increments
/// as elements in the set. Note that std::set may be too slow.
class Handler_registry {
  using Registry = std::vector<Event_handler*>;
public:
  using iterator       = Registry::iterator;
  using const_iterator = Registry::const_iterator;

  Handler_registry();

  // Registration
  void add(Event_handler*);
  void remove(Event_handler*);

  // Subscription
  void subscribe(Event_handler*, Event_mask);
  void unsubscribe(Event_handler*, Event_mask);

  int max() const;

  // Wait set
  const Select_set& wait() const;

  // Iterators
  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

private:
  void on_subscribe(Event_handler*);
  void on_unsubscribe(Event_handler*);

private:
  Registry  reg_;   // The registry
  int        max_;  // Maximum fd in the registry
  Select_set wait_; // Cached fdsets for the selector
};

} // namespace nocontrol

#include <freeflow/sys/handler.ipp>

#endif
