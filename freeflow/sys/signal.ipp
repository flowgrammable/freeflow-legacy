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

namespace freeflow {

// -------------------------------------------------------------------------- //
// Signal action

/// Construct a signal action whose handler is a nullptr. This can
/// be used to uninstall handlers for an action.
inline
Signal_action::Signal_action()
  : Signal_action((Signal_fn)nullptr) { }


/// Copy the curently installed signal action for signal s into this
/// object. An exception is thrown if this fails for any reason.
inline
Signal_action::Signal_action(int s) {
  System_result r = ::sigaction(s, nullptr, this);
  if (not r)
    throw std::runtime_error(strerror(errno));
}

/// Construct a signal action with the signal handler f.
inline
Signal_action::Signal_action(Signal_fn f) {
  sigfillset(&sa_mask);
  sa_flags = 0;
  sa_handler = f;
}

/// Construct a signal action with the signal information function f.
inline
Signal_action::Signal_action(Signal_info_fn f) {
  sigfillset(&sa_mask);
  sa_flags = SA_SIGINFO;
  sa_sigaction = f;
}

/// Allows explicit conversion to bool. An action converts to true
/// iff its handler function is non-null.
inline
Signal_action::operator bool() const {
  if (sa_flags & SA_SIGINFO)
    return sa_sigaction;
  else
    return sa_handler;
}

/// Install this action for the signal s.
inline System_result
Signal_action::install(int s) {
  return ::sigaction(s, this, nullptr);
}


// -------------------------------------------------------------------------- //
// Signal mask

inline
Signal_set::Signal_set(Init f) {
  if (f == NONE)
    sigemptyset(&mask);
  else if (f == ALL)
    sigfillset(&mask);
}

inline void 
Signal_set::insert(int s) { sigaddset(&mask, s); }

inline void 
Signal_set::remove(int s) { sigdelset(&mask, s); }

inline bool 
Signal_set::test(int s) { return sigismember(&mask, s); }

inline Signal_set 
Signal_set::all() { return {ALL}; }

inline Signal_set 
Signal_set::none() { return {NONE}; }

/// Returns the process's current signal mask.
inline Signal_set
Signal_set::current() {
  Signal_set m;
  ::sigprocmask(SIG_SETMASK, nullptr, m);
  return m;
}

inline
Signal_set::operator sigset_t*() { return &mask;}

inline
Signal_set::operator const sigset_t*() const { return &mask; }

} // namespace freeflow
