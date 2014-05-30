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

#ifndef FREEFLOW_SIGNAL_HPP
#define FREEFLOW_SIGNAL_HPP

#include <csignal>
#include <cstring>

#include <freeflow/sys/resource.hpp>

namespace freeflow {

using Signal_fn = void(*)(int);
using Signal_info_fn = void(*)(int, siginfo_t*, void*);

/// The Signal_action class describes behaviors to be taken when a
/// signal is sent to ths proces.
///
/// This class is a simple wrapper on sigation that provides initialization
/// semantics.
///
/// \todo Allow the specification of flags, etc.
struct Signal_action : sigaction {
  Signal_action();
  Signal_action(int);
  Signal_action(Signal_fn);
  Signal_action(Signal_info_fn);

  System_result install(int);

  explicit operator bool() const;
};

/// The Signal_mask type provides a means of specifying which
/// signals should be blocked or not.
struct Signal_set {
  enum Init { NONE, ALL, DEFAULT };

  Signal_set(Init = DEFAULT);

  void insert(int);
  void remove(int);
  bool test(int);

  static Signal_set all();
  static Signal_set none();
  static Signal_set current();

  operator sigset_t*();
  operator const sigset_t*() const;

  sigset_t mask;
};


} // namespace freeflow 

#include "signal.ipp"

#endif
