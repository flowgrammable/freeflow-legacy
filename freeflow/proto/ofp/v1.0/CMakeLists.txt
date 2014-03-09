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

set(src message.cpp port.cpp action.cpp)

set(hdr message.hpp 
        message.ipp
        port.hpp
        port.ipp)

add_library(freeflow-ofp-v1.0 ${src})

# ---------------------------------------------------------------------------- #
# Installation

install(FILES ${hdr} DESTINATION include/freeflow/proto/ofp/v1.0)

