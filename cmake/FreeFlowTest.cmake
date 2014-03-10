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

enable_testing()

# Add a unit test the given target and file name. Note that unit tests
# must be prefixed sys_${mod}, where ${mod} is the name of the module
# for which the test is written.
#
# Necessary libraries are listed following the name of the source
# file. For example:
#
#     add_unit_test(test_string string.cpp freeflow freeflow-ofp)
#
# Libraries can also be passed as a list variable.
macro(add_unit_test tgt file)
  add_executable(${tgt} ${file})
  target_link_libraries(${tgt} ${ARGN})
  add_test(test_${tgt} ${tgt})
endmacro()
