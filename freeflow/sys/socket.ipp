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

#include <iostream>

namespace freeflow {

// -------------------------------------------------------------------------- //
// Ipv4

inline
Ipv4_addr::Ipv4_addr(in_addr_t n) {
  s_addr = n;
}

inline
Ipv4_sockaddr::Ipv4_sockaddr(const Ipv4_addr& a, Ip_port p) {
  sin_family = IP4;
  sin_port = htons(p);
  sin_addr = a;
}

inline Ip_port 
Ipv4_sockaddr::port() const { return sin_port; }

inline Ipv4_addr& 
Ipv4_sockaddr::addr() { 
  return static_cast<Ipv4_addr&>(sin_addr); 
}

inline const Ipv4_addr& 
Ipv4_sockaddr::addr() const { 
  return static_cast<const Ipv4_addr&>(sin_addr); 
}

inline bool 
operator==(const Ipv4_addr& a, const Ipv4_addr& b) {
  return a.s_addr == b.s_addr;
}

inline bool 
operator!=(const Ipv4_addr& a, const Ipv4_addr& b) {
  return a.s_addr != b.s_addr;
}

inline bool 
operator==(const Ipv4_sockaddr& a, const Ipv4_sockaddr& b) {
  return a.port() == b.port() and a.addr() == b.addr();
}

inline bool 
operator!=(const Ipv4_sockaddr& a, const Ipv4_sockaddr& b) {
  return not(a == b);
}

// -------------------------------------------------------------------------- //
// Ipv6

inline
Ipv6_addr::Ipv6_addr(in6_addr n) {
  ::memcpy(this, &n, sizeof(in6_addr));
}

inline
Ipv6_sockaddr::Ipv6_sockaddr(const Ipv6_addr& a, Ip_port p) {
  sin6_family = IP6;
  sin6_port = htons(p);
  sin6_flowinfo = 0;
  sin6_addr = a;
  sin6_scope_id = 0;
}

inline Ip_port 
Ipv6_sockaddr::port() const { 
  return sin6_port; 
}

inline Ipv6_addr& 
Ipv6_sockaddr::addr() {
  return static_cast<Ipv6_addr&>(sin6_addr);
}

inline const Ipv6_addr& 
Ipv6_sockaddr::addr() const {
  return static_cast<const Ipv6_addr&>(sin6_addr);
}

inline bool 
operator==(const Ipv6_addr& a, const Ipv6_addr& b) {
  return not memcmp(a.s6_addr, b.s6_addr, sizeof(Ipv6_addr));
}

inline bool operator!=(const Ipv6_addr& a, const Ipv6_addr& b) {
  return not(a == b);
}

inline bool 
operator==(const Ipv6_sockaddr& a, const Ipv6_sockaddr& b) {
  return a.port() == b.port() and a.addr() == b.addr();
}

inline bool 
operator!=(const Ipv6_sockaddr& a, const Ipv6_sockaddr& b) {
  return not(a == b);
}

// -------------------------------------------------------------------------- //
// Address

namespace {

// Convert the string to an address of the given type. This directly
// populates the underlyinh object by passing it as a sockaddr pointer.
inline void
init_addr(const std::string& str, Address& a) {
  int result = inet_pton(a.family(), str.c_str(), a.addr());
  if (result <= 0) {
    if (result == 0)
      throw std::runtime_error("invalid address string");
    else
      throw system_error();
  }
}

// Assign the port to the given address. 
inline void
init_port(Ip_port p, Address& a) {
  switch (a.family()) {
  case Address::IP4: a.as_ipv4().sin_port = p; break;
  case Address::IP6: a.as_ipv6().sin6_port = p; break;

  // FIXME: Throw a more intuitive error?
  default: throw std::runtime_error("unknown address family");
  }
}

} // namespace

// TODO: Zero initializing the storage results in redundant assignments.
// In general, this is necessary since POSIX only specifies the required
// fields and not all fields. On some systems, we may be able to omit
// the zero initialization since the sockaddr_in* contain only the required
// fields. Very, very system-dependent optimization.

inline
Address::Address(Family f, const std::string& a, Ip_port p)
    : storage() // Zero-initialize the storage
{
  storage.ss_family = f;
  init_port(p, *this);
  init_addr(a, *this);
}

inline
Address::Address(const Ipv4_addr& a, Ip_port p) {
  new (&storage) Ipv4_sockaddr(a, p);
}

inline
Address::Address(const Ipv6_addr& a, Ip_port p) {
  new (&storage) Ipv6_sockaddr(a, p);
}

inline Address::Family
Address::family() const { return Family(storage.ss_family); }

inline Ipv4_sockaddr&
Address::as_ipv4() {
  return *reinterpret_cast<Ipv4_sockaddr*>(&storage);
}

inline const Ipv4_sockaddr&
Address::as_ipv4() const {
  return *reinterpret_cast<const Ipv4_sockaddr*>(&storage);
}

inline Ipv6_sockaddr&
Address::as_ipv6() {
  return *reinterpret_cast<Ipv6_sockaddr*>(&storage);
}

inline const Ipv6_sockaddr&
Address::as_ipv6() const {
  return *reinterpret_cast<const Ipv6_sockaddr*>(&storage);
}

inline sockaddr*
Address::addr() { 
  return reinterpret_cast<sockaddr*>(&storage); 
}

inline const sockaddr*
Address::addr() const { 
  return reinterpret_cast<const sockaddr*>(&storage); 
}

/// Returns the size of the underlying address.
inline socklen_t
Address::len() const { 
  if (family() == Address::IP4)
    return sizeof(Ipv4_sockaddr);
  else if (family() == Address::IP6)
    return sizeof(Ipv6_sockaddr);
  else
    throw std::runtime_error("unknown address family");
}


// Two addresses compare equal when they are in the same address family
// and represent the same socket addresses.
inline bool
operator==(const Address& a, const Address& b) {
  if (a.storage.ss_family == b.storage.ss_family) {
    if (a.family() == Address::IP4)
      return a.as_ipv4() == b.as_ipv4();
    else if (a.family() == Address::IP6)
      return a.as_ipv6() == b.as_ipv6();
    else
      throw std::runtime_error("unknown address family");
  } else {
    return false;
  }
}

inline bool
operator!=(const Address& l, const Address& r) { 
  return not (l==r); 
}

// -------------------------------------------------------------------------- //
// Socket

inline
Socket_info::Socket_info(Family f, Transport t)
  : family(f), transport(t)
{ }

inline
Socket_info::Socket_info(Transport t, const Address& l, const Address& p)
  : transport(t), local(l), peer(p)
{ }

/// Return the socket type 
inline int
Socket_info::type(Transport t)
{
  switch(t) {
    case Socket_info::TCP:
    case Socket_info::SCTP:
    case Socket_info::TLS:
      return SOCK_STREAM;
    case Socket_info::UDP:
      return SOCK_DGRAM;
    default:
      return SOCK_RAW;
  }
}

inline int
Socket_info::type() const { return type(transport); }


inline int
Socket_info::protocol(Transport t)
{
  switch(t) {
    case Socket_info::TCP:
    case Socket_info::TLS:
    case Socket_info::RAW_TCP:
      return IPPROTO_TCP;
    
    case Socket_info::UDP:
    case Socket_info::RAW_UDP:
      return IPPROTO_UDP;
    
    // case Socket_info::SCTP:
    //   return IPPROTO_SCTP;
    
    case Socket_info::RAW_ICMPv4:
      return IPPROTO_ICMP;
    
    case Socket_info::RAW_ICMPv6:
      return IPPROTO_ICMPV6;
    
    default:
      return IPPROTO_RAW;
  }
}

inline int
Socket_info::protocol() const { return protocol(transport); }

inline
Socket::Socket(Socket&& s)
  : Socket_info(s), Resource(std::move(s))
{ }

inline Socket&
Socket::operator=(Socket&& s) {
  Socket_info::operator=(s);
  Resource::operator=(std::move(s));
  return *this;
}

inline
Socket::Socket(Family f, Transport t)
  : Socket_info(f, t), Resource(::socket(family, type(), protocol()))
{
  if (fd() < 0)
    throw system_error();
}

inline
Socket::Socket(int f, Transport t, const Address& l, const Address& p)
  : Socket_info(t, l, p), Resource(f)
{ }

inline System_error
Socket::bind(const Address& a) {
  local = a;
  return ::bind(fd(), local.addr(), local.len());
}

inline System_error
Socket::connect(const Address& a) {
  peer = a;
  return ::connect(fd(), peer.addr(), peer.len());
}

inline System_error
Socket::listen(int backlog) {
  backlog = backlog;
  return ::listen(fd(), backlog);
}

inline Socket
Socket::accept() {
  socklen_t n;
  int s = ::accept(fd(), peer.addr(), &n);
  if (s < 0)
    throw System_error(s);
  return Socket(s, transport, local, peer);
}

inline std::size_t
Socket::read(void* buf, std::size_t n) {
  ssize_t r = ::read(fd(), buf, n);
  if (r < 0)
    throw System_error(r);
  return r;
}

inline std::size_t
Socket::write(const void* buf, std::size_t n) {
  ssize_t r = ::write(fd(), buf, n);
  if (r < 0)
    throw System_error(r);
  return r;
}

inline std::size_t
Socket::recv(void* buf, std::size_t n, int f) {
  ssize_t r = ::recv(fd(), buf, n, f);
  if (r < 0)
    throw System_error(r);
  return r;  
}

inline std::size_t
Socket::recv_from(void* buf, std::size_t n, Address& a) {
  socklen_t l;
  ssize_t r = ::recvfrom(fd(), buf, n, 0, a.addr(), &l);
  if (r < 0)
    throw System_error(r);
  return r;
}

inline std::size_t
Socket::send(const void* buf, std::size_t n, int f) {
  ssize_t r = ::send(fd(), buf, n, f);
  if (r < 0)
    throw System_error(r);
  return r;
}

inline std::size_t
Socket::send_to(const void* buf, std::size_t n, const Address& a) {
  ssize_t r = ::sendto(fd(), buf, n, 0, a.addr(), a.len());
  if (r < 0)
    throw System_error(r);
  return r;
}

} // namespace freeflow
