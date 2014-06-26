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

/// Extract the nth bit from the number passed in.
/// \todo where to move this?
inline bool
get_bit(int number, int n) { return (number & (1 << n)) >> n; }



/// Set the features of the port based on the features contained
/// in an OpenFlow v1.0 Feature_reply message.
inline void
Port::Features::set_features(ofp::v1_0::Port::Features f) {

  // 10 MB half duplex
  if (get_bit(f, 0)) {
    speed = 10000;
    mode  = HALF_DUPLEX;
  }

  // 10 MB full duplex
  else if (get_bit(f, 1)) {
    speed = 10000;
    mode  = FULL_DUPLEX;
  }

  // 100 MB half dulex
  else if (get_bit(f, 2)) {
    speed = 100000; 
    mode  = HALF_DUPLEX;
  }

  // 100 MB full dulex
  else if (get_bit(f, 3)) {
    speed = 100000;
    mode  = FULL_DUPLEX;
  }
  
  // 1 GB half dulex
  else if (get_bit(f, 4)) {
    speed = 1000000;
    mode  = HALF_DUPLEX;
  }
  
  // 1 GB full dulex
  else if (get_bit(f, 5)) {
    speed = 1000000;
    mode  = FULL_DUPLEX;
  }

  // 10 GB full duplex
  else if (get_bit(f, 6)) {
    speed = 10000000;
    mode  = FULL_DUPLEX;
  }

  // else error? v1.3 uses max_speed or curr_speed but v1.0 doesn't have them

  // Copper medium
  if (get_bit(f, 7)) 
    medium = COPPER;

  // Fiber medium
  else if (get_bit(f, 8))
    medium = FIBER;

  // else error? is this data necessary enough to warrant an error?

  auto_neg   = get_bit(f, 9);
  pause      = get_bit(f, 10);
  pause_asym = get_bit(f, 11);
}

} // namespace freeflow