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

#include "buffer.hpp"

namespace freeflow {

// Reading

/// Reads the contents of the file indicated by str into a new buffer.
/// If reading fails, the resulting buffer is in a bad state.
Buffer
read(const char* str) {
  std::ifstream is(str, std::ios::binary);
  return read(is);
}

/// Reads the contents of the file indicated by str into a buffer.
/// If reading fails, the resulting buffer is in a bad state.
Buffer
read(const std::string& str) {
  return read(str.c_str());
}

/// Reads the contents of the given input stream into the buffer.
/// The buffer is created large enough to accommodate the contents
/// of the entire stream. If reading fails, the resulting buffer is 
/// in a bad state.
Buffer
read(std::ifstream& is) {
  Buffer buf;
  if (not is) {
    buf.bad();
  } else {
    is.seekg(0, std::ios::end);
    buf.resize(is.tellg());
    is.seekg(0, std::ios::beg);
    is.read((char*)buf.data(), buf.size());
  }
  return buf;
}

// Writing

/// Writes the contents of buf into the file indicated by str.
/// If writing the buffer fails, the function returns false.
bool
write(const Buffer& buf, const char* str) {
  std::ofstream os(str, std::ios::binary);
  return write(buf, os);
}

/// Writes the contents of buf into the file indicated by str.
// If writing the buffer fails, the function return false.
bool
write(const Buffer& buf, const std::string& str) {
  return write(buf, str.c_str());
}

/// Writes the contents of buf into the output stream os. 
/// If the buffer is in a bad state or the writing fails, 
/// If writing fails, then the failbit on the output stream is set.
bool
write(const Buffer& buf, std::ofstream& os) {
  if (not buf)
    os.setstate(std::ios::failbit);
  else
    os.write((char*)buf.data(), buf.size());
  return os;
}

} // namespace freeflow
