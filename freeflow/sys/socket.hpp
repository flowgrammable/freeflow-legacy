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

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#include <freeflow/sys/error.hpp>
#include <freeflow/proto/ipv4.hpp>
#include <freeflow/proto/ipv6.hpp>

namespace freeflow {
namespace socket {

/// The address class is a union of the various address family formats
/// supported by the host system.
///
/// TODO: Document me.
struct Address
{
  enum Type : sa_family_t { 
    IPv4 = AF_INET, 
    IPv6 = AF_INET6
  };

  Address(Type t = IPv4, const std::string& n = std::string(), uint16_t p = 0);
  Address(ipv4::Address a, uint16_t p = 0);
  Address(ipv6::Address a, uint16_t p = 0);

  Address(const Address& a);
  Address& operator=(const Address& a);

  sockaddr_storage storage;
};

bool operator==(const Address& l, const Address& r);
bool operator!=(const Address& l, const Address& r);

Address::Type family(const Address& a);
sockaddr* addr(const Address& a);
socklen_t len(const Address& a);

// Printing
std::string to_string(const Address& a);

/// The socket class wraps the system socket API. This class can be used
/// to create client and server sockets.
///
/// TODO: Document me!
struct Socket
{
  enum Transport { 
    UDP = SOCK_DGRAM, 
    TCP = SOCK_STREAM
  };

  Socket(int f, Transport t, const Address& l, const Address& p);
  Socket(Transport t, Address a = Address());

  Socket(Socket&& s);
  ~Socket();

  Address   local;
  Address   peer;
  Transport transport;
  int       fd;
  int       backlog;
};

// Socket operations
Error bind(Socket& s, Address a = Address());
Error connect(Socket& s, const Address& a);
Error listen(Socket& s, int backlog = SOMAXCONN);
Socket accept(Socket& s);

int read(Socket& s);
int write(Socket& s);
int sendto(Socket& s);
int recvfrom(Socket& s);

// Pretty printing
std::string to_string(const Socket& s);

} // namespace socket
} // namespace freeflow

#include "socket.ipp"

#endif
