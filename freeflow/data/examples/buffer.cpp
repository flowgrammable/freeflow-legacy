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

#include <freeflow/data/buffer.hpp>

using namespace std;
using namespace freeflow;

// Write the error message to stderr and return -1.
int 
fail(const char* msg) {
  cerr << "error: " << msg << '\n';
  return -1;
}

int 
main(int argc, char* argv[]) {
  // Check arguments.
  if (argc != 3) {
    cerr << "error: not enough arguments\n";
    cerr << "usage: buffer <input-file> <output-file>\n";
  }

  // Reading a buffer from a file is done using the read() function. This opens
  // the given path name (in binary mode) and copies the contents into the
  // buffer. The buffer is automatically resized to accommodate the entire
  // contents of the input file.
  //
  // If the file cannot be read for any reason (e.g., the file does not
  // exist), then the resulting buffer is in a partially formed state; no
  // operations are valid except assignment and destruction.
  Buffer buf = read(argv[1]);
  if (not buf)
    return fail("could not read buffer");
  std::cout << "read buffer of " << buf.size() << " bytes\n";

  // Writing a buffer to a file is done using the write() function. This opens
  // the given path name (in binary mode) and copies the contents of the
  // the buffer into the output file.
  //
  // If the output file cannot be written for any reason, the write function
  // returns false.
  if (not write(buf, argv[2]))
    return fail("could not write buffer");
  std::cout << "wrote buffer to file\n";
}
