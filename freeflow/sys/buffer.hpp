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

#ifndef FREEFLOW_BUFFER_HPP
#define FREEFLOW_BUFFER_HPP

#include <cassert>
#include <cstdint>
#include <algorithm>
#include <iosfwd>

/// \file buffer.hpp
/// Resources for memory buffers.

namespace freeflow {

// Declarations
class Buffer;

/// A standard octet.
using Byte = uint8_t;

// -------------------------------------------------------------------------- //
// Buffer

/// A block of memory containing uninterpreted data.
///
/// The Buffer is block of memory that stores data read from or to be
/// written to a network device. Reading and writing is primarily done through
/// the View class.
class Buffer : public std::vector<Byte> {
public:
  // Default construction
  Buffer() = default;

  // Move semantics
  Buffer(Buffer&& x) = default;
  Buffer& operator=(Buffer&& x) = default;

  // Copy semantics
  Buffer(const Buffer& x) = default;
  Buffer& operator=(const Buffer& x) = default;

  // Fill initialization
  explicit Buffer(std::size_t n, Byte value = Byte());

  // Range initialization
  Buffer(const Byte* first, const Byte* last);
};

// Reading
Buffer read(const char* s);
Buffer read(const std::string& s);
Buffer read(std::ifstream& s);
std::size_t read(Buffer& b, std::ifstream& is);

// Writing
void write(const Buffer& b, const char* s);
void write(const Buffer& b, const std::string& s);
void write(const Buffer& b, std::ofstream& os);

// Protocol
std::size_t bytes(const Buffer& b);
constexpr bool valid(const Buffer& b);

// -------------------------------------------------------------------------- //
// View

/// The primary interface for reading from and writing to Buffers.
///
/// The View class is a reference to a bounded segment of memory within
/// a buffer. Reading and writng of network messages is done through the
/// buffer using the get() and put() operations. Each successive read and
/// write operation advances the bounded range of memory. 
///
/// There are a number of operations associated with Views.
///   * The remaining() function indicates the number of bytes in the bounded
///     range that can be read or written. 
///   * The available() operation determines
///     if there are enough bytes remaining to complete such an operation. 
///   * The constrain() operation returns another view whose end occurs
///     before the original (i.e., has fewer bytes).
///
/// Note that it is undefined behavior to read from or write to a view when
/// there are not enough bytes to complete the operation.
class View {
public:
  View(Buffer& b);
  View(Buffer& b, std::size_t n);
  View(Buffer& b, Byte* f, Byte* l);

  std::size_t remaining() const;
  bool available(std::size_t n) const;

  Buffer& buf;
  Byte* first;
  Byte* last;
};

inline View constrain(const View& v, std::size_t n);

inline bool update(View& v, const View& c);

// Put operations
template<typename T>
  void put(View& v, const T& x) = delete;

inline void put(View& v, char c);
inline void put(View& v, uint8_t n);
inline void put(View& v, uint16_t n);
inline void put(View& v, uint32_t n);
inline void put(View& v, uint64_t n);

template<typename T>
  inline void put(View& v, T* p, std::size_t n);

// Get operations
template<typename T>
  void get(View& v, T& x) = delete;

inline void get(View& v, char& c);
inline void get(View& v, uint8_t& n);
inline void get(View& v, uint16_t& n);
inline void get(View& v, uint32_t& n);
inline void get(View& v, uint64_t& n);

template<typename T>
  inline T get(View& v);

template<typename T>
  inline void
  get(View& v, T* p, std::size_t n);

// Padding
inline void pad(View& v, std::size_t n);


} // namespace freeflow

#include "buffer.ipp"

#endif
