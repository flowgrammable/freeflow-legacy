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
Buffer::Buffer(std::size_t n, Byte b)
  : std::vector<Byte>(n, b)
{ }

inline
Buffer::Buffer(const Byte* first, const Byte* last)
  : std::vector<Byte>(first, last)
{ }

// -------------------------------------------------------------------------- //
// View

/// Initialize a view over the extent of the buffer.
inline
View::View(Buffer& b)
  : buf(b), first(b.data()), last(first + b.size())
{ }

/// Initialize the view over the first n bytes of the buffer.
inline
View::View(Buffer& b, std::size_t n)
  : buf(b), first(b.data()), last(b.data() + n)
{
  assert(n <= b.size());
}

/// Initialize the view over a subset of bytes in the buffer.
///
/// \todo Assert that f is in b, l is in b, and f < l. 
inline
View::View(Buffer& b, Byte* f, Byte* l)
  : buf(b), first(f), last(l)
{ }

/// Returns the number of bytes remaining in the view.
inline std::size_t
View::remaining() const { return last - first; }

/// Returns true if and only if there are at least n bytes available in
/// the view for reading or writing.
inline bool
View::available(std::size_t n) const { return n <= remaining(); }

/// Advance the view by n bytes. Behavior is undefined if n is greater 
/// than the number of bytes remaining.
inline void
View::advance(std::size_t n) {
  assert(available(n));
  first += n;
} 

/// Returns a new, constrained view of the buffer containing excatly n bytes. 
/// A view is typically constrained whenever a new length field is encountered
/// in a protocol. This guarantees that interpeting data from the following
/// bytes does not exceed a boundary.
///
/// Note that behavior is undefined if the the constructed view is not within
/// the view v.
inline View
View::constrain(std::size_t n) const {
  assert(available(n));
  return View(buf, first, first + n);
}

/// Returns the number of buytes remaining in the view.
inline std::size_t
remaining(const View& v) { return v.remaining(); }

// -------------------------------------------------------------------------- //
// View constraint

inline 
View_constraint::View_constraint(View& v, std::size_t n) 
  : view(v), length(v.available(n) ? n : -1)
{ }

inline View
View_constraint::compose() const {
  return View(view.buf, view.first, view.first + length);
}

/// Returns a view constraint.
inline View_constraint 
constrain(View& v, std::size_t n) { return View_constraint(v, n); }

// -------------------------------------------------------------------------- //
// Constrained view

inline
Constrained_view::Constrained_view(View_constraint c) 
  : constraint(c), view(c.compose())
{ }

inline
Constrained_view::~Constrained_view() {
  if (*this)
    constraint.view.advance(constraint.length);
}

inline
Constrained_view::operator View&() { return constraint.view; }

inline
Constrained_view::operator bool() const { 
  return constraint.length != std::size_t(-1); 
}

// -------------------------------------------------------------------------- //
// Get and put


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
put(View& v, Uint8 n) { detail::put_value(v, n); }

/// Write a 16-bit data into a view.
inline void
put(View& v, Uint16 n) { detail::put_value(v, n); }

/// Write 32-bit data into a view.
inline void
put(View& v, Uint32 n) { detail::put_value(v, n); }

/// Write 64-bit data into a view.
inline void
put(View& v, Uint64 n) { detail::put_value(v, n); }

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
get(View& v, Uint8& n) { detail::get_value(v, n); }

/// Get a 16-bit datum from a view.
inline void
get(View& v, Uint16& n) { detail::get_value(v, n); }

/// Get a 32-bit datum from a view.
inline void
get(View& v, Uint32& n) { detail::get_value(v, n); }

/// Get a 64-bit datum from a view.
inline void
get(View& v, Uint64& n) { detail::get_value(v, n); }

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
