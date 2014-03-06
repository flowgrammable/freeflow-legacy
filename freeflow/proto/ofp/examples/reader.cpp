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

#include <iostream>

#include <freeflow/sys/file.hpp>
#include <freeflow/sys/buffer.hpp>

#include <freeflow/proto/ofp/ofp.hpp>

using namespace freeflow;

void print(const File&, const ofp::Header& h);
void diagnose(const File&, const ofp::Header&, Error);

int main(int argc, char* argv[]) {
  if (argc == 1) {
    std::cerr << "usage: reader <files>\n"; 
    return -1;
  }

  // Create a buffer that will store only a single header.
  Buffer buf(8);

  // Read a message from each of the paths given in the command
  // line arguments.

  for (int i = 1; i < argc; ++i) {
    // Open the ith file for reading. If the file cannot be opened,
    // an exception is thrown.
    try {
      File f(argv[i], File::READ);

      // Read some among of data into the buffer.
      //
      // FIXME: This only works for small files. We actually need a
      // buffered file that can correctly pull data from a file or
      // buffer when more data is requested.
      std::size_t n = f.read(buf);

      // Create a view over the n bytes read from the buffer.
      View v(buf, n);

      // Try to construct a header from the given buffer. If successful, 
      // print information about the header. Otherwise, print a diagnostic
      // indicating the reason that parsing failed.
      ofp::Header h;
      if (Trap t = from_view(v, h)) {
        diagnose(f, h, t.error());
        continue;
      }

      // Print the header
      print(f, h);

      // TODO: Read the remainder buffer and decode it.

    } catch(Error& e) {
      // FIXME: This interface should be improved.
      ::perror("error");
      continue;
    }
  }
}

void
print(const File& f, const ofp::Header& h) {
  std::cout << "== " << f.path() << " ==\n";
  std::cout << "version: " << (int)h.version << '\n';
  std::cout << "type:    " << (int)h.type << '\n';
  std::cout << "length:  " << h.length << '\n';
  std::cout << "xid:     " << h.xid << '\n';
}

void 
diagnose(const File& f, const ofp::Header& h, Error err) {
  std::cout << "error: " << f.path() << ": ";

  // If there wasn't enough data for the header, then we can't
  // read any parts of the header.
  if (err.code() == ofp::Error::HEADER_OVERFLOW) {
    std::cout << "header overflow\n";
    return;
  }

  // Check the header length.
  if (err.code() == ofp::Error::BAD_HEADER_LENGTH) {
    std::cout << "bad header length\n";
    return;
  }

  // ... Do stuff.
  if (h.version > 1) {
    std::cout << "unhandled protocol version '" << (int)h.version << "'\n";
    return;
  }
}

