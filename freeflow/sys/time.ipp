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

constexpr Hours
operator"" _h(unsigned long long n) { return Hours(n); }

constexpr Minutes
operator"" _m(unsigned long long n) { return Minutes(n); }

constexpr Seconds
operator"" _s(unsigned long long n) { return Seconds(n); }

constexpr Milliseconds
operator"" _ms(unsigned long long n) { return Milliseconds(n); }

constexpr Microseconds
operator"" _us(unsigned long long n) { return Microseconds(n); }

constexpr Nanoseconds
operator"" _ns(unsigned long long n) { return Nanoseconds(n); }

/// Returns the current time, measured by the system's high 
/// resolution clock.
inline Time_point
now() { return std::chrono::high_resolution_clock::now(); }
  
} // namespace freeflow

