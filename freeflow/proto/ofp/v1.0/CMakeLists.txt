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

set(src port.cpp 
        queue.cpp 
        match.cpp 
        action.cpp 
        stats.cpp 
        message.cpp)

set(hdr error.hpp
        message.hpp message.ipp
        port.hpp    port.ipp
        queue.hpp   queue.ipp
        match.hpp   match.ipp
        action.hpp  action.ipp)


# ---------------------------------------------------------------------------- #
# Build

add_shared_library(freeflow-ofp-1.0 ${src})
target_link_libraries(freeflow-ofp-1.0 freeflow)


# ---------------------------------------------------------------------------- #
# Installation

install(TARGETS freeflow-ofp-1.0 LIBRARY DESTINATION lib)
install(FILES ${hdr} DESTINATION include/freeflow/proto/ofp/v1.0)

