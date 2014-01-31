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


/// Returns the number of bytes in the buffer.
inline std::size_t
bytes(const Buffer& b) { return b.size(); }


/// Returns true when the contents of the buffer can be accessed.
/// This function returns true only when the buffer is in a good state.
inline bool
is_valid(const Buffer& b) { return (bool)b; }

} // freeflow
