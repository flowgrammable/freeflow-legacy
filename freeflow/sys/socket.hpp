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
#include <type_traits>

#include <freeflow/sys/error.hpp>
#include <freeflow/proto/ipv4.hpp>
#include <freeflow/proto/ipv6.hpp>

namespace freeflow {
namespace socket {

// -------------------------------------------------------------------------- //
// Internet protocol support
//
// TODO: Consider moving this into a separate file.

/// A class used to encapsulate static information about address
/// families. This provides names for derived classes, to help improve
/// writability.
struct Address_info {
  enum Family : sa_family_t {
    IPv4 = AF_INET, 
    IPv6 = AF_INET6
  };
};

/// The type of an Internet port.
using Ip_port = in_port_t;

// =====
// Ipv4
// =====

/// An Ipv4 host adress.
struct Ipv4_addr : in_addr, Address_info { 
  static constexpr Family family = IPv4;
};

static_assert(sizeof(Ipv4_addr) == sizeof(in_addr), "non-conforming compiler");

/// An Ipv4 socket address is a host and port. This class provides a
/// read-only view of the underlying system structure.
struct Ipv4_sockaddr : sockaddr_in, Address_info {
  static constexpr Family family = IPv4;

  Ipv4_sockaddr() = default;
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
struct Ipv6_addr : in6_addr, Address_info {
  static constexpr Family family = IPv6;
};

/// An Ipv6 socket address is a host and port. This class provides a
/// read-only view of the underlying system structure.
struct Ipv6_sockaddr : sockaddr_in6, Address_info {
  static constexpr Family family = IPv6;

  Ipv6_sockaddr() = default;
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

// TODO: Move these assertions into a unit test.

// The implementation relies on the compiler's implementation of the
// empty base optimization. 
static_assert(sizeof(Ipv4_addr) == sizeof(in_addr), "");
static_assert(sizeof(Ipv4_sockaddr) == sizeof(sockaddr_in), "");
static_assert(sizeof(Ipv6_addr) == sizeof(in6_addr), "");
static_assert(sizeof(Ipv6_sockaddr) == sizeof(sockaddr_in6), "");

// Require that all of these classes are trivial types.
static_assert(std::is_trivial<Ipv4_addr>::value, "");
static_assert(std::is_trivial<Ipv4_sockaddr>::value, "");
static_assert(std::is_trivial<Ipv6_addr>::value, "");
static_assert(std::is_trivial<Ipv6_sockaddr>::value, "");

// -------------------------------------------------------------------------- //
// Socket address


/// The address class represents a socket address for a local or remote
/// host. The details of the address are determined by its address family.
/// Examples include IPv4 and IPv6 socket addresses, which are comprised
/// of a 32 or 128 bit host address and a 16 bit port number.
///
/// This is the primary interface for constructing and working with
/// socket addresses.
struct Address : Address_info {

  Address() = default;

  Address(Family t, const std::string& n, Ip_port p);
  Address(const Ipv4_addr& a, Ip_port p = 0);
  Address(const Ipv6_addr& a, Ip_port p = 0);

  Family family() const;

  Ipv4_sockaddr&       as_ipv4();
  const Ipv4_sockaddr& as_ipv4() const;
  
  Ipv6_sockaddr&       as_ipv6();
  const Ipv6_sockaddr& as_ipv6() const;

  sockaddr_storage storage;
};

bool operator==(const Address& l, const Address& r);
bool operator!=(const Address& l, const Address& r);

sockaddr*       addr(Address& a);
const sockaddr* addr(const Address& a);

socklen_t len(const Address& a);

// Printing
std::string to_string(const Address& a);

// TODO: Move all of these assertions into a unit test.

// Compiler guarantees
static_assert(std::is_trivial<sockaddr_storage>::value, "");
static_assert(std::is_trivial<Address>::value, "");


// -------------------------------------------------------------------------- //
// Socket

// The socket base class encapsulates informaiton about a socket. This
// is used to provide efficient move semantics for a Socket (since the
// state is a trivial type).
struct Socket_base : Address_info {
  enum Transport { 
    // Internet protocols
    UDP, 
    TCP,
    TLS,
    SCTP,
    
    // Raw sockets
    RAW_IPV4,
    RAW_IPV6,
    RAW_UDP,
    RAW_TCP,
  };

  Socket_base() = default;
  Socket_base(Family f, Transport t);
  Socket_base(int, Transport, const Address&, const Address&);

  Family    family;
  Transport transport;
  Address   local;
  Address   peer;
  int       fd;
  int       backlog;
};

static_assert(std::is_trivial<Socket_base>::value, "");

int socket_type(Socket_base::Transport t);
int socket_protocol(Socket_base::Transport t);

/// The socket is an endpoint for communicating systems.
///
/// Note that sockets are resources; they can be moved but not copied. 
///
/// TODO: Document me!
struct Socket : Socket_base {
  // Not default constructible.
  Socket() = delete;

  // Not copyable.
  Socket(const Socket&) = delete;
  Socket& operator=(const Socket&) = delete;

  // Move semantics
  Socket(Socket&&);
  Socket& operator=(Socket&&);

  // Socket construction
  Socket(Family, Transport);
  Socket(int, Transport, const Address&, const Address&);

  ~Socket();
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
