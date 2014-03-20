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

# CMake configuration.,
cmake_minimum_required(VERSION 2.8)
set(FREEFLOW_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${FREEFLOW_MODULE_PATH})

project(FreeFlow C CXX)

# ---------------------------------------------------------------------------- #
# Build options
#
# Project-specific options used to configure the build.

option(FREEFLOW_BUILD_TESTS "Build unit tests" ON)

# ---------------------------------------------------------------------------- #
# Environment configuration
#
# The inclusion of these modules configure the build based on the discovery
# of the host system.

include(FreeFlowHost)
include(FreeFlowCompiler)
include(FreeFlowDepend)
include(FreeFlowTest)
include(FreeFlowDoc)

# ---------------------------------------------------------------------------- #
# Build

add_subdirectory(freeflow)
add_subdirectory(nocontrol)

