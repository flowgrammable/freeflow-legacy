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
#include <iosfwd>
#include <vector>

#include <freeflow/sys/data.hpp>

/// \file buffer.hpp
/// Resources for memory buffers.

namespace freeflow {

// Declarations
class Buffer;

/// A standard octet.
using Byte = Uint8;

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

// -------------------------------------------------------------------------- //
// Views

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

  // Observers
  std::size_t remaining() const;
  bool available(std::size_t n) const;

  // Mutators
  void advance(std::size_t n);

  // Constrained views
  View constrain(std::size_t n) const;

  Buffer& buf;
  Byte* first;
  Byte* last;
};

/// A View_constraint is a helper class used to construct a constrained
/// view. It maintains information about the request for the constraint.
struct View_constraint {
  static constexpr std::size_t invalid = -1;

  View_constraint(View& v, std::size_t n);

  View compose() const;

  View& view;
  std::size_t length;
};

/// A Constrained view is a helper class that simplifies the process 
/// of constraining and updating views. Constructing a constrained view
/// creates a new view and associates with the parent view, constrained
/// to a given length. The view starts at the current position of the
/// parent and ranges over the specified length.
///
/// The constrained view can be contextually converted to bool. If the
/// new view cannot be constructed (because the parent does not have
/// enough bytes available), then the constrained view will evaluate to
/// false. A common use pattern is as follows:
///
///   if (Constrained_view c = constrain(v, n)) {
///     if (Trap err = from_view(c, x))
///       return err.code();
///   } else {
///     return OVERFLOW;
///   }
///
/// This program returns only in the case that an error as occurred. When
/// no errors are present, the program continues outside the block.
class Constrained_view {
public:
  Constrained_view(View_constraint);
  ~Constrained_view();

  operator View&();
  explicit operator bool() const;

  View_constraint constraint;
  View view;
};

// Operations
std::size_t remaining(const View&);
View_constraint constrain(View& v, std::size_t n);

// Put operations
template<typename T>
  void put(View& v, const T& x) = delete;

inline void put(View& v, char c);
inline void put(View& v, Uint8 n);
inline void put(View& v, Uint16 n);
inline void put(View& v, Uint32 n);
inline void put(View& v, Uint64 n);

template<typename T>
  inline void put(View& v, T* p, std::size_t n);

// Get operations
template<typename T>
  void get(View& v, T& x) = delete;

inline void get(View& v, char& c);
inline void get(View& v, Uint8& n);
inline void get(View& v, Uint16& n);
inline void get(View& v, Uint32& n);
inline void get(View& v, Uint64& n);

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
