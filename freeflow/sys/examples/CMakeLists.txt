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

add_executable(error error.cpp)
target_link_libraries(error freeflow_sys)

add_executable(client client.cpp)
target_link_libraries(client freeflow_sys)

add_executable(server server.cpp)
target_link_libraries(server freeflow_sys)

add_executable(server2 server2.cpp)
target_link_libraries(server2 freeflow_sys)

add_executable(console console.cpp)
target_link_libraries(console freeflow_sys)

add_executable(buffer buffer.cpp)
target_link_libraries(buffer freeflow_sys)

add_executable(buffer2 buffer2.cpp)
target_link_libraries(buffer2 freeflow_sys)

add_executable(view view.cpp)
target_link_libraries(view freeflow_sys)
