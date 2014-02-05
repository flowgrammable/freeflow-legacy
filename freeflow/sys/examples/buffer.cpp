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

  // Reading a buffer from a file is done using the read() function. This opens
  // the given path name (in binary mode) and copies the contents into the
  // buffer. The buffer is automatically resized to accommodate the entire
  // contents of the input file.
  //
  // If the file cannot be read for any reason (e.g., the file does not
  // exist), an exception will be thrown.
  Buffer buf = read(argv[1]);
  std::cout << "read buffer of " << buf.size() << " bytes\n";

  // Writing a buffer to a file is done using the write() function. This opens
  // the given path name (in binary mode) and copies the contents of the
  // the buffer into the output file.
  //
  // If the output file cannot be written for any reason, an exception will
  // be thrown.
  write(buf, argv[2]);
  std::cout << "wrote buffer to file\n";
}
