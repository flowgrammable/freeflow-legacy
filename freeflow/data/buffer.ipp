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

namespace freeflow {

inline 
Buffer::Buffer()
  : std::vector<Byte>(), state_(GOOD), missing_(0) 
{ }

inline 
Buffer::Buffer(Buffer&& x)
  : std::vector<Byte>(std::move(x)), state_(x.state_), missing_(x.missing_) 
{ }

inline Buffer&
Buffer::operator=(Buffer&& x) {
  std::vector<Byte>::operator=(std::move(x));
  state_ = x.state_;
  missing_ = x.missing_;
  return *this;
}

inline 
Buffer::Buffer(const Buffer& x)
  : std::vector<Byte>(x), state_(x.state_), missing_(x.missing_) 
{ }

inline Buffer&
Buffer::operator=(const Buffer& x) {
  std::vector<Byte>::operator=(x);
  state_ = x.state_;
  missing_ = x.missing_;
  return *this;
}

inline
Buffer::Buffer(std::size_t n)
  : std::vector<Byte>(n), state_(GOOD), missing_(0)
{ }

inline
Buffer::Buffer(const Byte* first, const Byte* last)
  : std::vector<Byte>(first, last), state_(GOOD), missing_(0)
{ }

inline void
Buffer::missing(std::size_t n)
{
  state_ = MISSING;
  missing_ = n;
}

inline void
Buffer::bad() { state_ = BAD; }


/// \ingroup structure
/// Returns the number of bytes in the buffer.
inline std::size_t
bytes(const Buffer& b) { return b.size(); }

/// \ingroup semantics
/// Returns true when the contents of the buffer can be accessed.
/// This function returns true only when the buffer is in a good state.
inline bool
is_valid(const Buffer& b) { return (bool)b; }


// -------------------------------------------------------------------------- //
// View

/// Initialize a view over the extent of the underlying buffer.
inline
View::View(Buffer& b)
  : buf(b), first(b.data()), last(first + b.size())
{ }

/// Initialize the view over a subset of bytes in the given buffer.
///
/// \todo Assert that f is in b, l is in b, and f < l.
inline
View::View(Buffer& b, Byte* f, Byte* l)
  : buf(b), first(f), last(l)
{ }

/// \defgroup view_ops Buffer View Operations

/// \ingroup view_ops
/// Returns the number of bytes remaining in the view.
inline std::size_t
remaining(const View& v) { return v.last - v.first; }

/// \ingroup view_ops
/// Returns true if and only if there are at least n bytes available in
/// the view for reading or writing.
//
// FIXME: Modfiy this so that it returns an error condition that evaluates
// to true when there n bytes are available and false otherwise, while
// reporting the number of bytes needed to complete the operation.
inline bool
available(const View& v, std::size_t n) { return remaining(v) >= n; }

/// \ingroup view_ops
/// Returns a new view of a buffer containing excatly n bytes. A view
/// is typically constrained whenever a new length field is encountered
/// in a protocol. This guarantees that interpeting data from the following
/// bytes does not exceed a boundary.
///
/// Note that behavior is undefined if the the constructed view is not within
/// the view v.
inline View
constrain(const View& v, std::size_t n) {
  assert(v.last - v.first >= std::ptrdiff_t(n));
  return View(v.buf, v.first, v.first + n);
}

/// \ingroup view_ops
/// Update a view v to the furthest position of a constrained view c
/// and returns true iff c is an empty view.
///
/// Note that behavior is undefined if the view c is not within the view v.
inline bool
update(View& v, const View& c) {
  // If we didn't fully advance through the constrained, view,
  // then we cannot update v.
  if (c.first != c.last)
    return false;

  // Advance to the new view.
  v.first = c.first;
  return true;
}


namespace detail {
template<typename T>
  inline void 
  put_value(View& v, T n) {
    assert(v.first + sizeof(T) <= v.last);
    *(reinterpret_cast<T*>(v.first)) = n;
    v.first += sizeof(T);
  }

template<typename T>
  inline T 
  get_value(View& v, T& n) {
    assert(v.first + sizeof(T) <= v.last);
    n = *reinterpret_cast<T*>(v.first);
    v.first += sizeof(T);
    return n;
  }
} // namespace detail

/// Write a character c into a view.
inline void
put(View& v, char c) { detail::put_value(v, c); }

/// Write an 8-bit data into a view.
inline void
put(View& v, uint8_t n) { detail::put_value(v, n); }

/// Write a 16-bit data into a view.
inline void
put(View& v, uint16_t n) { detail::put_value(v, n); }

/// Write 32-bit data into a view.
inline void
put(View& v, uint32_t n) { detail::put_value(v, n); }

/// Write 64-bit data into a view.
inline void
put(View& v, uint64_t n) { detail::put_value(v, n); }

// Write an n-element array pointed to by p into a view.
template<typename T>
  inline void
  put(View& v, T* p, std::size_t n) {
    while (n) { 
      put(v, *p);
      ++p;
      --n;
    }
  }

/// Get a character from a view.
inline void
get(View& v, char& c) { detail::get_value(v, c); }

/// Get an 8-bit datum from a view.
inline void
get(View& v, uint8_t& n) { detail::get_value(v, n); }

/// Get a 16-bit datum from a view.
inline void
get(View& v, uint16_t& n) { detail::get_value(v, n); }

/// Get a 32-bit datum from a view.
inline void
get(View& v, uint32_t& n) { detail::get_value(v, n); }

/// Get a 64-bit datum from a view.
inline void
get(View& v, uint64_t& n) { detail::get_value(v, n); }

// Get an object of type T from a view if and only if T is a type
// for which get() is defined.
template<typename T>
  inline T
  get(View& v) {
    T n;
    get(v, n);
    return n;
  }

/// Get an n-element array of of objects of type T from a view if and
/// only if T is a type for which get() is defined.
template<typename T>
  inline void
  get(View& v, T* p, std::size_t n) {
    while (n) {
      get(v, *p);
      ++p;
      --n;
    }
  }

/// Read or write n bytes of padding to a view. Note that no data is
/// actually read or written; the view is simply advanced.
inline void
pad(View& v, std::size_t n) { 
  assert(v.first + n <= v.last);
  v.first += n;
}

} // freeflow
