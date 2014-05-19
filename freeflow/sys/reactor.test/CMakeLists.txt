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

set(libs freeflow)

add_executable(sys_reactor_accept accept.cpp)
target_link_libraries(sys_reactor_accept ${libs})

add_executable(sys_reactor_connect connect.cpp)
target_link_libraries(sys_reactor_connect ${libs})

add_executable(sys_reactor_timer timer.cpp)
target_link_libraries(sys_reactor_timer ${libs})
