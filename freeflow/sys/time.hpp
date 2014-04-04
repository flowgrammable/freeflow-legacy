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

#ifndef FREEFLOW_TIME_HPP
#define FREEFLOW_TIME_HPP

#include <chrono>

namespace freeflow {

/// A time point is a unit of time on the system's high-resolution
/// timer. It represents the duration since the epoch.
using Time_point = std::chrono::high_resolution_clock::time_point;

/// A duration represents a unit of time (e.g., 3 seconds).
using Duration = std::chrono::high_resolution_clock::duration;

// Units of time
using Hours        = std::chrono::hours;
using Minutes      = std::chrono::minutes;
using Seconds      = std::chrono::seconds;
using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;
using Nanoseconds  = std::chrono::nanoseconds;

// User-defined literals.
constexpr Hours operator"" _h(unsigned long long);
constexpr Minutes operator"" _m(unsigned long long);
constexpr Seconds operator"" _s(unsigned long long);
constexpr Milliseconds operator"" _ms(unsigned long long);
constexpr Microseconds operator"" _us(unsigned long long);
constexpr Nanoseconds operator"" _ns(unsigned long long);

// Operations
Time_point now();


} // namespace freeflow

#include "time.ipp"

#endif
