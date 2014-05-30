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

#ifndef FREEFLOW_REACTOR_HPP
#define FREEFLOW_REACTOR_HPP

#include <freeflow/sys/handler.hpp>
#include <freeflow/sys/timer.hpp>

namespace freeflow {

namespace impl {
// A helper class for initializing reactor data.
struct Reactor_init {
  Reactor_init();

  static void init_signals();
};
} // namespace impl

/// The Reactor class implements an event processor that notifies handlers
/// of resource events and availability, timer expiration, and signals.
///
/// \todo This should probably be renamed Select_reactor and more tightly
/// coupled with the handler registry.
class Reactor : private impl::Reactor_init {
  friend class Reactor_init;
public:
  Reactor();

  // Handler registration
  void add_handler(Event_handler*);
  void remove_handler(Event_handler*);
  void remove_handlers();

  template<typename T, typename... Args> 
    T* new_handler(Args&&...);

  void new_handler(Event_handler*);

  // Event subscription
  void subscribe_events(Event_handler*, Event_mask);
  void unsubscribe_events(Event_handler*, Event_mask);

  // Timers
  void schedule_timer(Event_handler*, int, Microseconds);
  void reschedule_timer(Event_handler*, int, Microseconds);
  void cancel_timer(Event_handler*, int);

  // Control
  void run();
  void run(Microseconds);
  void stop();

private:
  void notify_signal(Resource_set&);
  void notify_select(const Select_set&, Resource_set&);
  void notify_timers(Resource_set&);
  void close_handlers(const Resource_set&);

private:
  /// The running flag is true until the reactor has stopped.
  bool             running_;
  
  /// Maintains the list of handlers regstered to the reactor.
  Handler_registry handlers_;
  
  /// The timer queue provides an ordering of timers.
  Timer_queue      timers_;
  
  /// The timer list maintains timers that have triggered since an
  /// expiration time. These are kept to prevent unnecessary
  /// reallocations of elements in the list.
  Timer_list       expired_;  // List of triggered timers
};

} // namesapce freeflow

#include <freeflow/sys/reactor.ipp>

#endif
