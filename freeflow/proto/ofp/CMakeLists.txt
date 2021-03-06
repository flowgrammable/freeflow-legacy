# Copyright (c) 2013-2014 Flowgrammable.org
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
# Build

set(src ofp.cpp error.cpp)

set(hdr error.hpp error.ipp
        ofp.hpp   ofp.ipp)


# ---------------------------------------------------------------------------- #
# Targets

add_shared_library(freeflow-ofp ${src})
target_link_libraries(freeflow-ofp freeflow)


# ---------------------------------------------------------------------------- #
# OpenFlow Versoin

add_subdirectory(v1.0)
add_subdirectory(v1.1)
add_subdirectory(v1.2)
add_subdirectory(v1.3.0)


# ---------------------------------------------------------------------------- #
# Examples

add_subdirectory(examples)


# ---------------------------------------------------------------------------- #
# Testing

add_subdirectory(ofp.test)


# ---------------------------------------------------------------------------- #
# Installation

install(TARGETS freeflow-ofp LIBRARY DESTINATION lib)
install(FILES ${hdr} DESTINATION include/freeflow/proto/ofp)
