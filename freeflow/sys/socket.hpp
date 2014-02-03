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

#ifndef FREEFLOW_SOCKET_HPP
#define FREEFLOW_SOCKET_HPP

extern "C" {
#include <sys/socket.h>
#include <in/inet.h>
}

namespace freeflow {
namespace socket {

struct Address
{
  enum Type { 
    IPv4 = AF_INET, 
    IPv6 = AF_INET6
  };

  union {
    sockaddr_in ipv4;
    sockaddr_in6 ipv6;
  };

  Type type;
};

std::string to_string(const Address& a);

struct Socket
{
  enum Type { 
    UDP = SOCK_DGRAM, 
    TCP = SOCK_STREAM
  };

  Socket(Socket&& s);
  Socket(Transport t);
  Socket(Transport t, Address a);
  ~Socket();

  Address addr;
  int fd;
};

bool bind(Socket& s);
bool connect(Address a);

bool listen(Socket& s);
bool accept(Socket& s);

bool read(Socket& s);
bool wriet(Socket& s);
bool sendto(Socket& s);
bool recvfrom(Socket& s);

bool close(Socket& s);

std::string to_string(const Socket& s);


} // namespace socket
} // namespace freeflow

#include "socket.ipp"

#endif