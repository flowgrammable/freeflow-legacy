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

inline
File_status::File_status()
  : stat() // Zero-initialize the file buffer.
{ }


inline
File::File(File&& f)
  : fd(f.fd), path(f.path), stat(f.stat)
{
  f.fd = -1;
}

inline File&
File::operator=(File&& f) {
  fd = f.fd;
  path = f.path;
  stat = f.stat;
  f.fd = -1;
  return *this;
}

inline
File::File(const char* p)
  : fd(::open(p, O_RDONLY)), path(p), stat()
{
  if (fd < 0)
    throw system_error();
}

inline
File::File(const Path& p)
  : File(p.c_str()) 
{ }

// TODO: Cache the file status on the first read.
inline const File_status&
File::status() {
  int err = ::fstat(fd, &stat);
  if (err < 0)
    throw system_error();
  return stat;
}

} // namespace freeflow
