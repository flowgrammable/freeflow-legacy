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

#include <string>

#include <freeflow/sys/error.hpp>

namespace freeflow {

/// A Path is a sequence of characters that designates a file object.
///
/// @todo This should be an actual class with a legitimate interface.
/// Or, we could rely on the C++ File System TS whenever it's implemented
/// standardized.
using Path = std::string;


// The File status class represents information about a file.
struct File_status : stat {
  File_status();

  dev_t device() const { return st_dev; }
};


/// The File class provides read/write access to an underlying file
/// descriptor.
struct File {

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
  // FIXME: What mode should the file be created in?
  File(const char* p);
  File(const std::string& p);


  // Return the file status.
  const File_status& status();

  bool has_status()

  int fd;
  std::string path;
  File_status stat;
};

} // namespace freeflow

#include <freeflow/sys/file.ipp>

#endif

