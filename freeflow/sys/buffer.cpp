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

#include <fstream>
#include <string>

#include "buffer.hpp"

namespace freeflow {

// Reading

/// Reads the contents of the file indicated by str into a new buffer.
/// An exception is thrown if the file cannot be opened.
Buffer
read(const char* s) {
  std::ifstream is(s, std::ios::binary);
  if (not is)
    throw std::runtime_error("could not open '" + std::string(s) + '\'');
  return read(is);
}

/// Reads the contents of the file indicated by str into a buffer.
/// An exception is thrown if the file cannot be opened.
Buffer
read(const std::string& s) {
  return read(s.c_str());
}

/// Reads the contents of the given input stream into the buffer. The
/// stream must be in a good state, otherwise behavior is undefined.
Buffer
read(std::ifstream& is) {
  Buffer buf;
  is.seekg(0, std::ios::end);
  buf.resize(is.tellg());
  is.seekg(0, std::ios::beg);
  is.read((char*)buf.data(), buf.size());
  return buf;
}

/// Reads contents from the input stream into the buffer. The number of
/// bytes read depends on the size of the buffer. The number of successfully
/// read bytes is returned. Not that behavior is undefined if the stream is
/// in an initially bad state.
std::size_t
read(Buffer& b, std::ifstream& is) {
  is.read((char*)b.data(), b.size());
  return is.gcount();
}

// Writing

/// Writes the contents of buf into the file indicated by str.
/// An exception is thrown if the file cannot be opened.
void
write(const Buffer& buf, const char* str) {
  std::ofstream os(str, std::ios::binary);
  if (not os)
    throw std::runtime_error("could not open '" + std::string(str) + '\'');
  return write(buf, os);
}

/// Writes the contents of buf into the file indicated by str.
/// An exception is thrown if the file cannot be opened.
void
write(const Buffer& buf, const std::string& str) {
  return write(buf, str.c_str());
}

/// Writes the contents of buf into the output stream os. The stream must
/// be in a good state, otherwise behavior is undefined.
void
write(const Buffer& buf, std::ofstream& os) {
  os.write((char*)buf.data(), buf.size());
}

} // namespace freeflow
