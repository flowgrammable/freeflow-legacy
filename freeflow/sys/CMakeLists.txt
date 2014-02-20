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

set(src error.cpp
        fd_utils.cpp
        file.cpp
        socket.cpp
        buffer.cpp
        signal.cpp
        time.cpp
        selector.cpp
        scheduler.cpp
        syslog.cpp
        pipe.cpp)

set(bsd_src kqueue.cpp)
set(linux_src epoll.cpp)

if(BSD)
  LIST(APPEND src ${bsd_src})
endif(BSD)

if(LINUX)
  LIST(APPEND src ${linux_src})
endif(LINUX)

set(inc error.hpp
        fd_utils.hpp
        file.hpp
        socket.hpp
        buffer.hpp
        signal.hpp
        time.hpp
        selector.hpp
        scheduler.hpp
        syslog.hpp
        pipe.hpp)

set(bsd_inc kqueue.hpp)
set(linux_inc epoll.hpp)

if(BSD)
  LIST(APPEND inc ${bsd_inc})
endif(BSD)

if(LINUX)
  LIST(APPEND inc ${linux_inc})
endif(LINUX)

add_library(freeflow_sys ${src})

# --------------------------------------------------------------------------- //
# Installation
#
# TODO: Install the library!

install(FILES ${inc} DESTINATION include/freeflow/sys)

# --------------------------------------------------------------------------- //
# Examples and docs

add_subdirectory(examples)
