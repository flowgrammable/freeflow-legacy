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

#ifndef FREEFLOW_FILE_HPP
#define FREEFLOW_FILE_HPP

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/path.hpp>

namespace freeflow {

/// The Resource class is the base class of all POSIX resources that
/// are interanally represented by a file descriptor.
class Resource {
public:
  // The default resource is an invalid file descriptor.
  Resource();

  /// Move semantics. Moving a resources causes the original to be
  /// invalidated.
  Resource(Resource&& r);
  Resource& operator=(Resource&& r);

  // Resource initialization
  explicit Resource(int f);

  // Destructor
  ~Resource();

  /// Evaluates to true only when the resource is valid.
  explicit operator bool() const { return fd_ >= 0; }

  /// Returns the file descriptor.
  int fd() const { return fd_; }

private:
  int fd_;
}; 


// -------------------------------------------------------------------------- //
// File status

/// The Status class represents information about a resource. 
struct Status : stat {
  Status() = default;

  Status(const char*);
  Status(const Path&);
  Status(const Resource&);

  dev_t device() const { return st_dev; }
};

/// The Link status class is used to initialize status information about
/// a symbolic link.
///
/// Note that Link status cannot be constructed from an open resource.
struct Link_status : Status {
  Link_status(const char*);
  Link_status(const Path&);
  Link_status(const Resource&) = delete;
};


// -------------------------------------------------------------------------- //
// File objects


/// The File class encapsulates a set of basic services for reading from,
/// writing to, and creating files.
///
/// Note that not all operations are valid for every kind of file or
/// device. For example, if working with a symbolic link, use the Link
/// class; if working with a FIFO, use the Pipe class.
class File : public Resource {
public:
  using Flags = int;

  /// Open the file for only reading.
  static constexpr Flags READ = O_RDONLY;

  /// Open the file for only writing.
  static constexpr Flags WRITE = O_WRONLY;

  /// Open the file for both reading and writing. Note that this flag is
  /// not the same as specifying READ | WRITE.
  static constexpr Flags RW = O_RDWR;

  /// Opening a file that does not exist will result in the creation
  /// of a new file. Otherwise, nothing happens.
  static constexpr Flags CREATE = O_CREAT;

  /// Will only open a file that does not exist.
  static constexpr Flags NEW = O_CREAT | O_EXCL;

  /// The length of the file is set to 0.
  static constexpr Flags TRUNCATE = O_TRUNC;

  // TODO: Define async and other flags.


  // A file cannot be default constructed.
  File() = delete;

  // Move semantics.
  File(File&&);
  File& operator=(File&&);

  // Files are non-copyable
  File(const File&) = delete;
  File& operator=(const File&) = delete;

  // Open a file in read/write mode.
  //
  // FIXME: If CREATE or NEW is specified, then the constructor
  // must take a mode_t argument. We should have a Creator class
  // that encapsulates file creation patterns (not unlike Acceptor
  // and Connector).
  File(const char*, Flags);
  File(const Path&, Flags);

  // TODO: Add read/write operatoin.

private:
  Path path_;
};


} // namespace freeflow

#include <freeflow/sys/file.ipp>

#endif

