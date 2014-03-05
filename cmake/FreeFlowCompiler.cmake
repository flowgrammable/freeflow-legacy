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
# Compiler configuration
#
# Minimally, -std=c++11 and -Wall must be enabled.

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fPIC -g")


# For Clang-3.3 installed via MacPorts on MacOS 10.6.8, also be sure to
# include the following:
#
#   -stdlib=libc++ -U__STRICT_ANSI__
#
# Currently, this must be done by setting CXXFLAGS or setting this variable
# using ccmake. Note unsetting __STRICT_ANSI__ will cause some checks to
# be disabled.

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -U__STRICT_ANSI__")

# TODO: Be smarter about recognizing compilers and operating systems, and
# configure this automatically.
