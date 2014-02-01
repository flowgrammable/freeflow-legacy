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
    cerr << "usage: view <input-file> <output-file>\n";
    return -1;
  }

  // Read the input file into a buffer. 
  Buffer in = read(argv[1]);

  // Attach a view to the input buffer in order to get data from the
  // buffer. Using the view to read from the buffer helps guarantee that
  // a read does not cross a memory boundary (e.g., outside the bounds
  // of the buffer or a packet in the buffer).
  //
  // Note that all get() operations on a buffer have undefined behavior 
  // when if the operation crosses a memory boundary. For example, getting
  // 4 bytes of data when only 2 bytes remain in the view. It is the
  // responsibility of the caller to ensure that the read will succeed
  // before reading.
  View vin(in);
  if (not available(vin, sizeof(uint32_t)))
    return fail("buffer is not big enough to read from");
  
  // Read 4 bytes of data from the file and print it. This particular
  // get() operation is provided for convenience. It is equivalent
  // to writing this:
  //
  //    uint32_t n;
  //    get(vin, n);
  //
  // We prefer the one-line version.
  uint32_t n = get<uint32_t>(vin);
  std::cout << "read value: " << n << '\n';
  std::cout << "bytes remaining: " << remaining(vin) << '\n';


  // Create an output buffer large to hold only 4 bytes and a attach a
  // view for writing. As with the get() operations, put() operations are
  // also constrained by the view. That is, trying to write 4 bytes of
  // data into a view with only 2 bytes remaining will result in undefined
  // behavior. Here, we can dispense with the validation because the view
  // is guaranteed to have sufficient availability (failure to allocate
  // memory results in an exception).
  Buffer out(sizeof(n));
  View vout(out);
  put(vout, n);

  // Write the buffer to an output file.
  write(out, argv[2]);
  std::cout << "wrote buffer to file\n";
}
