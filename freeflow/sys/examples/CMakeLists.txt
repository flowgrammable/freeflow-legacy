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

macro(add_example tgt)
  add_executable(${tgt} ${tgt}.cpp)
  target_link_libraries(${tgt} freeflow_sys)
endmacro()

add_example(error)
add_example(file)
add_example(buffer)
add_example(buffer2)
add_example(view)

add_example(selector)
add_example(pipe)
add_example(signal)
add_example(scheduler)
add_example(server)

if(BSD)
  add_example(kqueue)
endif()
