// Copyright (c) 2014 Flowgrammable, LLC.
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

#ifndef FREEFLOW_FD_UTILS_HPP
#define FREEFLOW_FD_UTILS_HPP

extern "C" {
#include <unistd.h>
}

#include <cstdint>

/// \file sys/utilities.hpp

namespace freeflow {

struct Transfer
{
  Transfer(int f, uint8_t* b, std::size_t n);

  operator int();

  int fd;
  uint8_t* buff;
  std::size_t n_bytes;

  int fd_state;
  std::size_t n_transferred;
};

int read_available(Transfer& t);
int read_all(Transfer& t);

int write_available(Transfer& t);
int write_all(Transfer& t);

/// Read n bytes from fd to buff
/// Loops on read until complete or failure
inline int
spin_read(int fd, uint8_t* buff, int n);

/// Write n bytes from fd to buff
/// Loops on write until complete or failure
inline int
spin_write(int fd, uint8_t* buff, int n);

} // namespace freeflow

#include "fd_utils.ipp"

#endif
