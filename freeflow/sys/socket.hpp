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

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>

#include <freeflow/sys/error.hpp>
#include <freeflow/proto/ipv4.hpp>
#include <freeflow/proto/ipv6.hpp>

namespace freeflow {
namespace socket {

// -------------------------------------------------------------------------- //
// Internet protocol support
//
// TODO: Consider moving this into a separate file.

using Addres_family = sa_family_t;

/// The type of an Internet port.
using Ip_port = in_port_t;

// =====
// Ipv4
// =====

/// An Ipv4 host adress.
struct Ipv4_addr : in_addr { 
  static constexpr Addres_family family = AF_INET;

  Ipv4_addr(const Ipv4_addr&);
  Ipv4_addr& operator=(const Ipv4_addr&);
};

/// An Ipv4 socket address is a host and port. This class provides a
/// read-only view of the underlying system structure.
struct Ipv4_sockaddr : sockaddr_in {
  static constexpr Addres_family family = AF_INET;

  Ipv4_sockaddr(const Ipv4_sockaddr&);
  Ipv4_sockaddr& operator=(const Ipv4_sockaddr&);

  Ipv4_sockaddr(const Ipv4_addr&, Ip_port);
  Ipv4_sockaddr(const std::string&, Ip_port);

  Ip_port port() const;
  
  Ipv4_addr&       addr();
  const Ipv4_addr& addr() const;
};

bool operator==(const Ipv4_addr& a, const Ipv4_addr& b);
bool operator!=(const Ipv4_addr& a, const Ipv4_addr& b);

bool operator==(const Ipv4_sockaddr& a, const Ipv4_sockaddr& b);
bool operator!=(const Ipv4_sockaddr& a, const Ipv4_sockaddr& b);

// =====
// Ipv6
// =====

/// In Ipv6 host address.
struct Ipv6_addr : in6_addr {
  static constexpr Addres_family family = AF_INET6;

  Ipv6_addr(const Ipv6_addr&);
  Ipv6_addr& operator=(const Ipv6_addr&);
};

/// An Ipv6 socket address is a host and port. This class provides a
/// read-only view of the underlying system structure.
struct Ipv6_sockaddr : sockaddr_in6 {
  static constexpr Addres_family family = AF_INET6;

  Ipv6_sockaddr(const Ipv6_sockaddr&);
  Ipv6_sockaddr& operator=(const Ipv6_sockaddr&);

  Ipv6_sockaddr(const Ipv6_addr&, Ip_port);
  Ipv6_sockaddr(const std::string&, Ip_port);

  Ip_port port() const;

  Ipv6_addr&       addr();
  const Ipv6_addr& addr() const;
};

bool operator==(const Ipv6_addr& a, const Ipv6_addr& b);
bool operator!=(const Ipv6_addr& a, const Ipv6_addr& b);

bool operator==(const Ipv6_sockaddr& a, const Ipv6_sockaddr& b);
bool operator!=(const Ipv6_sockaddr& a, const Ipv6_sockaddr& b);


// -------------------------------------------------------------------------- //
// Socket address


/// The address class is a union of the various address family formats
/// supported by the host system.
///
/// This is the primary interface for constructing and working with
/// socket addresses.
///
/// TODO: Document me.
struct Address {
  enum Type : sa_family_t { 
    IPv4 = AF_INET, 
    IPv6 = AF_INET6
  };

  Address(const Address& a);
  Address& operator=(const Address& a);

  Address();
  Address(Type t, const std::string& n, Ip_port p);
  Address(Ipv4_addr a, Ip_port p = 0);
  Address(Ipv6_addr a, Ip_port p = 0);

  Type family() const;

  const Ipv4_sockaddr& as_ipv4() const;
  const Ipv6_sockaddr& as_ipv6() const;

  sockaddr_storage storage;
};

bool operator==(const Address& l, const Address& r);
bool operator!=(const Address& l, const Address& r);

Address::Type family(const Address& a);
sockaddr* addr(const Address& a);
socklen_t len(const Address& a);

// Printing
std::string to_string(const Address& a);


// -------------------------------------------------------------------------- //
// Socket

/// The socket class wraps the system socket API. This class can be used
/// to create client and server sockets.
///
/// TODO: Document me!
struct Socket
{
  enum Transport { 
    UDP = SOCK_DGRAM, 
    TCP = SOCK_STREAM,
    RAW_IPV4,
    RAW_IPV6,
    RAW_UDP,
    RAW_TCP,
  };

  Socket(Transport t);
  Socket(Transport t, const Address& a);
  Socket(int f, Transport t, const Address& l, const Address& p);

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
