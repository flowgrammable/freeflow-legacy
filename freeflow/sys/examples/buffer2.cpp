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

#include <iostream>
#include <fstream>
#include <string>

#include <freeflow/sys/buffer.hpp>

using namespace std;
using namespace freeflow;

// Write the error message to stderr and return -1.
int 
fail(const std::string& msg) {
  cerr << "error: " << msg << '\n';
  return -1;
}

int 
main(int argc, char* argv[]) {
  // Check arguments.
  if (argc != 3) {
    cerr << "error: not enough arguments\n";
    cerr << "usage: buffer <input-file> <output-file>\n";
    return -1;
  }
  
  // Open the input file.
  ifstream is(argv[1]);
  if (not is)
    return fail("coult not open '" + string(argv[1]) + '\'');

  // Create a buffer that can be filled with, at most, 64 bytes of data.
  constexpr size_t N = 64;
  Buffer buf(N);

  // Try filling the buffer from the input stream.
  std::size_t n = read(buf, is);
  if (n == N) {
    cout << "read 64 byte buffer\n";
  } else {
    // Print the a message indicating how much data was read, and then
    // resize the buffer is it contains only that data.
    cout << "read " << n << " bytes into buffer\n";
    buf.resize(n);
  }

  // Write to the output file.
  write(buf, argv[2]);
}
