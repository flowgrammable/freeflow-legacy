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

find_package(Doxygen 1.8.4)

# If Doxygen is installed, generate a doc target. Note that the 
# Doxyfile is configured from the Doxtfile.in file in the top-level 
# directory.
#
# TODO: Make a macro for adding documentation to a local directory.
# In essence an add_docs() macro that generates a unique documentation
# index.
if(DOXYGEN_FOUND)
  execute_process(
    COMMAND ${DOXYGEN_EXECUTABLE} --version
    OUTPUT_VARIABLE DOXYGEN_VERSION
    ERROR_VARIABLE DOXYGEN_VERSION
    RESULT_VARIABLE DOXYGEN_version_result)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in 
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} 
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()
