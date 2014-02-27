// Copyright (c) 2013-2014 Flowgrammable.org
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an "AS IS"
// BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing
// permissions and limitations under the License.

#include <freeflow/sys/socket.hpp>

// Test general requirements of the socket class.
//
// If any of these tests fail, then compiler is non-conforming.

using namespace freeflow;

// Guarantee that we have not accidentally increased the size
// of these classes.
static_assert(sizeof(Ipv4_addr) == sizeof(in_addr), "");
static_assert(sizeof(Ipv4_sockaddr) == sizeof(sockaddr_in), "");
static_assert(sizeof(Ipv6_addr) == sizeof(in6_addr), "");
static_assert(sizeof(Ipv6_sockaddr) == sizeof(sockaddr_in6), "");

// Guarantee that all address classes are, in fact, trivial.
static_assert(std::is_trivial<Ipv4_addr>::value, "");
static_assert(std::is_trivial<Ipv4_sockaddr>::value, "");
static_assert(std::is_trivial<Ipv6_addr>::value, "");
static_assert(std::is_trivial<Ipv6_sockaddr>::value, "");

// Guarantee tha storage and address are trivial.
static_assert(std::is_trivial<sockaddr_storage>::value, "");
static_assert(std::is_trivial<Address>::value, "");

// The socket base also.
static_assert(std::is_trivial<Socket_info>::value, "");

int main() { }
