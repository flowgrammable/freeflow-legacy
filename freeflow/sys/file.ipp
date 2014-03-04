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

namespace freeflow {

/// Construct status information about the file designated by the
/// given path name. If path designates a symbolic link, then the 
/// status information pertains to the linked file, and not the 
/// link.
inline
Status::Status(const char* p) {
  if (::stat(p, this))
    throw system_error();
}

/// Construct status information about the file designated by the
/// given path name. If path designates a symbolic link, then the 
/// status information pertains to the linked file, and not the 
/// link.
inline
Status::Status(const Path& p)
  : Status(p.c_str())
{ }

/// Construct status information about the given resource.
inline
Status::Status(const Resource& r) {
  if (::fstat(r.fd(), this))
    throw system_error();
}


/// Construct information about file designated by the given path.
/// If p designates a symbolic link, the status describes the link,
/// and not the linked file.
Link_status::Link_status(const char* p) {
  if (::lstat(p, this))
    throw system_error();
}

/// Construct information about file designated by the given path.
/// If p designates a symbolic link, the status describes the link,
/// and not the linked file.
Link_status::Link_status(const Path& p)
  : Link_status(p.c_str()) { }


// -------------------------------------------------------------------------- //

inline
File::File(File&& f)
  : Resource(std::move(f)), path_(f.path_)
{ }

inline File&
File::operator=(File&& f) {
  Resource::operator=(std::move(f));
  path_= std::move(f.path_);
  return *this;
}

inline
File::File(const char* p, Flags f)
  : Resource(::open(p, f)), path_(p)
{
  if (not *this)
    throw system_error();
}

inline
File::File(const Path& p, Flags f)
  : File(p.c_str(), f)
{ }

inline std::size_t
File::read(void* buf, std::size_t n) {
  ssize_t r = ::read(fd(), buf, n);
  if (r < 0)
    throw system_error();
  return r;
}

inline std::size_t
File::write(const void* buf, std::size_t n) {
  ssize_t r = ::write(fd(), buf, n);
  if (r < 0)
    throw system_error();
  return r;
}

} // namespace freeflow
