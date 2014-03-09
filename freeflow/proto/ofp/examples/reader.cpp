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
#include <freeflow/proto/ofp/v1.0/message.hpp>

using namespace freeflow;

void construct(File&, const ofp::Header&);
void construct_v1_0(const File&, const ofp::Header&, View&);
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
      if (f.read(buf) < buf.size()) {
        std::cerr << "error: could not read header\n";
        continue;
      }

      // Create a view over the n bytes read from the buffer.
      View v(buf);

      // Try to construct a header from the given buffer. If successful, 
      // print information about the header. Otherwise, print a diagnostic
      // indicating the reason that parsing failed.
      ofp::Header h;
      if (Trap t = from_view(v, h)) {
        diagnose(f, h, t.error());
        continue;
      }

      // Print the header
      construct(f, h);

      // TODO: Read the remainder buffer and decode it.

    } catch(Error& e) {
      // FIXME: This interface should be improved.
      ::perror("error");
      continue;
    }
  }
}

void
construct(File& f, const ofp::Header& h) {
  Buffer buf(h.length);
  if (f.read(buf) < h.length - h.bytes) {
    std::cerr << "error: failed to read message content from '" 
              << f.path() << "'\n";
    return;
  }
  View v = buf;

  switch(h.version) {
  case 1:
    construct_v1_0(f, h, v);
    break;
  default:
    std::cerr << "error: unhandled protocol version '" << h.version << "'\n";
    break;
  }
}

// TODO: Just read and pretty print the message.
void
construct_v1_0(const File& f, const ofp::Header& h, View& v) {
  using namespace ofp::v1_0;

  // TODO: This should not be in a try block. Also, I need to validate
  // that h.type is valid within the context of the protocol version.
  try {
    Message m(Message::Type(h.type));
    switch (m.type) {
    case HELLO: std::cout << "Hello\n"; break;
    case ERROR: std::cout << "Error\n"; break;
    case ECHO_REQUEST: std::cout << "Echo request\n"; break;
    case ECHO_REPLY: std::cout << "Echo reply\n"; break;
    default:
      std::cerr << "error: unhandled message\n";
    }
  } catch(...) {
    std::cerr << "error: unhandled message '" << (int)h.type << "'\n";
  }
}

void 
diagnose(const File& f, const ofp::Header& h, Error err) {
  std::cerr << "error: " << f.path() << ": ";

  // If there wasn't enough data for the header, then we can't
  // read any parts of the header.
  if (err.code() == ofp::Error::HEADER_OVERFLOW) {
    std::cerr << "error: header overflow\n";
    return;
  }

  // Check the header length.
  if (err.code() == ofp::Error::BAD_HEADER_LENGTH) {
    std::cerr << "error: bad header length\n";
    return;
  }

  // ... Do stuff.
  if (h.version > 1) {
    std::cerr << "error: unhandled protocol version '" << (int)h.version << "'\n";
    return;
  }
}

