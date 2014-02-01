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

#include "utilities.hpp"

namespace freeflow {

/////////////////////////////////////////////////////////////////////
//
// Reads n bytes from a file descriptor to a buffer or fails.
//
// This is a wrapper reading from stream oriented file descriptors.
// The low-level read interface is not always intuitive to users
// especially when using stream oriented transports such as TCP. This
// interface simplifies the process of using these interfaces when it
// is ok to block.
//
// The low-level read interface does not garuntee that n bytes will
// be read from the descriptor. It only garuntees that it will attempt
// to read n bytes, return on failure or with the number of bytes that
// was successfully read. This number may be < 0 to indicate failure, 
// = 0 to indicate end of file or a closed socket, or > 0 to indicate some
// number of bytes read. The number of bytes read will be <= n.
//
// In the case when the nubmer of bytes read is < n this function will 
// loop and try to read the remaining data. This loop will continue until
// the requested amount of data, n, has been read, it encounters an error,
// or it reaches the end of the stream (EOF or close).
//
/////////////////////////////////////////////////////////////////////

inline int
spin_read(int fd, uint8_t* buff, int n)
{
  auto fd_state = 1;  // Current state of fd
  int n_read = 0;     // Number of bytes successfully read to buff

  // More to read and fd state good
  while(n_read < n and fd_state > 0) {
    // Read the remainder to the buff
    fd_state = ::read(fd, buff+n_read, n-n_read);
    // If successful update bytes read
    if (fd_state > 0)
      n_read += fd_state;
  }
  // Return fd state on failure or number of bytes read
  return fd_state < 0 ? fd_state : n_read;
}

/////////////////////////////////////////////////////////////////////
//
// Writes n bytes to a file descriptor from a buffer or fails.
//
// This is a wrapper writing to stream oriented file descriptors.
// The low-level write interface is not always intuitive to users
// especially when using stream oriented transports such as TCP. This
// interface simplifies the process of using these interfaces when it
// is ok to block.
//
// The low-level write interface does not garuntee that n bytes will
// be written to the descriptor. It only garuntees that it will attempt
// to write n bytes, return on failure or with the number of bytes that
// was successfully written. This number may be < 0 to indicate failure, 
// = 0 to indicate end of file or a closed socket, or > 0 to indicate some
// number of bytes written. The number of bytes written will be <= n.
//
// In the case when the nubmer of bytes written is < n this function will 
// loop and try to write the remaining data. This loop will continue until
// the requested amount of data, n, has been written, it encounters an error,
// or it reaches the end of the stream (EOF or close).
//
/////////////////////////////////////////////////////////////////////

inline int
spin_write(int fd, uint8_t* buff, int n)
{
  auto fd_state = 1;  // Current state of fd
  int n_written = 0;  // Number of bytes successfully written from buff
  
  // More to write and fd state good
  while(n_written < n and fd_state > 0) {
    // Write the remainder from the buff
    fd_state = ::write(fd, buff+n_written, n-n_written);
    // If successful update bytes written
    if(fd_state > 0)
      n_written += fd_state;
  }
  // Return fd state on failure or number of bytes written
  return fd_state < 0 ? fd_state : n_written;
}


} // namespace freeflow

#endif
