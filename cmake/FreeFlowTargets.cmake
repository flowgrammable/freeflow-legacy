# Copyright (c) 2013-2014 Flowgrammable, LLC.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an "AS IS"
# BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
# or implied. See the License for the specific language governing
# permissions and limitations under the License.

# ---------------------------------------------------------------------------- #
# Shared libraries

## Creates a shared library from a set of source files and links against
## the given libariries.
##
## Synopsis:
##
##    add_shared_library(tgt src)
##
## Arguments:
##
##    tgt -- The name of the library target
##    src -- The set of input source files
##
## TODO: Allow the specification of libraries to link against.
##
macro(add_shared_library tgt src libs)
  add_library(${tgt} SHARED ${src})
  set_target_properties(${tgt} PROPERTIES VERSION ${FREEFLOW_VERSION})
endmacro(add_shared_library)
