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

# ---------------------------------------------------------------------------- #
# OpenSSL
#
# FreeFlow uses OpenSSL to provide TLS security to establish connections 
# between switches and controllers. 

find_package(OpenSSL)

# TODO: Not every package requries OpenSSL. In fact, it's only only 
# currently required when using the system library to connect to an
# OpenFlow switch or Controller. Define variables that include appropriate
# header files and set required libraries for linking.

