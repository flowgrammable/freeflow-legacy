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

/// \file sys/utilities.hpp

namespace freeflow {

struct Read
{
  Read(int f, uint8_t* b, int n);

  void operator()();
  operator int();

  int fd_state;
  uint8_t* buff;
  int n_bytes;
  int n_read;
};

struct Write
{
  Write(int f, uint8_t* b, int n);

  void operator()();
  operator int();

  int fd_state;
  uint8_t* buff;
  int n_bytes;
  int n_written;
};

/// Read n bytes from fd to buff
/// Loops on read until complete or failure
inline int
spin_read(int fd, uint8_t* buff, int n);

/// Write n bytes from fd to buff
/// Loops on write until complete or failure
inline int
spin_write(int fd, uint8_t* buff, int n);

#include "fd_utils.ipp"

} // namespace freeflow

#endif
