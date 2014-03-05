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

#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <freeflow/sys/data.hpp>

using namespace freeflow;

template<typename T>
  void 
  print(T n) {
    // Copy the value into a buffer and null-terminate it.
    char buf[sizeof(T)];
    std::memcpy(buf, &n, sizeof(T));

    // Print the integer values, possibly with a leading 0.
    for (int i = 0; i < sizeof(T); ++i)
      std::cout << std::setw(2) << std::setfill('0') << (int)buf[i];
    std::cout << '\n';
  }


int main() {
  // Create network byte-order values...
  char buf[] = {1, 2, 3, 4, 5, 6, 7, 8};

  Uint16 be16;
  std::memcpy(&be16, buf, 2);

  Uint32 be32;
  std::memcpy(&be32, buf, 4);

  Uint64 be64;
  std::memcpy(&be64, buf, 8);
  
  // Test that conversions are correct.

  Uint16 a16 = 0x0102;
  assert(Byte_order::msbf(a16) == be16);
  
  Uint32 a32 = 0x01020304;
  assert(Byte_order::msbf(a32) == be32);
  
  Uint64 a64 = 0x0102030405060708;
  assert(Byte_order::msbf(a64) == be64);

  // TODO: Test little endian coversions
}
