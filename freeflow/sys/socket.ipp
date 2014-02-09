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
namespace socket {

// TODO: Is memcpy really a valid means of copying sockaddr structures?

// -------------------------------------------------------------------------- //
// Utility

namespace {

// Convert the string to an address of the given type.
template<typename Addr>
inline void
init_addr(const std::string& str, Addr& a) {
  int result = inet_pton(a.family, str.c_str(), &a);
  if (result <= 0) {
    if (result == 0)
      // throw std::runtime_error("invalid address string");
      throw system_error();
    else
      throw system_error();
  }
}

} // namespace


// -------------------------------------------------------------------------- //
// Ipv4

inline
Ipv4_sockaddr::Ipv4_sockaddr(const Ipv4_addr& a, Ip_port p) {
  sin_family = family;
  sin_port = htons(p);
  addr() = a;
}

inline
Ipv4_sockaddr::Ipv4_sockaddr(const std::string& a, Ip_port p) {
  sin_family = family;
  sin_port = htons(p);
  init_addr(a, addr());
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
Ipv6_sockaddr::Ipv6_sockaddr(const std::string& a, Ip_port p) {
  sin6_family = AF_INET6;
  sin6_port = htons(p);
  sin6_flowinfo = 0;
  sin6_scope_id = 0;
  init_addr(a, addr());
}

inline
Ipv6_sockaddr::Ipv6_sockaddr(const Ipv6_addr& a, Ip_port p) {
  sin6_family = AF_INET6;
  sin6_port = htons(p);
  sin6_flowinfo = 0;
  sin6_scope_id = 0;
  addr() = a;
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

// TODO: Memsetting the storage results in double-initialization of
// sockaddr members. If we find we're doing this a lot, we may want
// to find ways to reduce the overhead.

inline
Address::Address(Family f, const std::string& n, Ip_port p) {
  // FIXME: Don't use placement new. We've already constructed
  // that memory (and initialized it), so we're technically 
  // double-constructing the storage object.
  if (f == IPv4)
    new (&storage) Ipv4_sockaddr(n, p);
  else if (f == IPv6)
    new (&storage) Ipv6_sockaddr(n, p);
  else
    throw std::runtime_error("unknown address family");
}

inline
Address::Address(Ipv4_addr a, Ip_port p) {
  new (&storage) Ipv4_sockaddr(a, p);
}

inline
Address::Address(Ipv6_addr a, Ip_port p) {
  new (&storage) Ipv6_sockaddr(a, p);
}

inline Address::Family
Address::family() const {
  return Family(storage.ss_family);
}

inline const Ipv4_sockaddr&
Address::as_ipv4() const {
  return *reinterpret_cast<const Ipv4_sockaddr*>(&storage);
}

inline const Ipv6_sockaddr&
Address::as_ipv6() const {
  return *reinterpret_cast<const Ipv6_sockaddr*>(&storage);
}

// Two addresses compare equal when they are in the same address family
// and represent the same socket addresses.
inline bool
operator==(const Address& a, const Address& b) {
  if(a.storage.ss_family == b.storage.ss_family) {
    if (a.family() == Address::IPv4)
      return a.as_ipv4() == b.as_ipv4();
    else if (a.family() == Address::IPv6)
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

/// Returns a pointer to the underlying socket address structure.
inline sockaddr*
addr(Address& a) { return reinterpret_cast<sockaddr*>(&a.storage); }

/// Returns the size of the underlying address.
inline socklen_t
len(const Address& a)  { 
  if (a.family() == Address::IPv4)
    return sizeof(Ipv4_sockaddr);
  else if (a.family() == Address::IPv6)
    return sizeof(Ipv6_sockaddr);
  else
    throw std::runtime_error("unknown address family");
}

// -------------------------------------------------------------------------- //
// Socket

// FIXME: I'm not sure we should have a constructor that selects an
// address fammily as a default.
inline
Socket::Socket(Family f, Transport t)
  : family(f), transport(t)
{
  fd = ::socket(family, transport, 0);
  if (fd < 0)
    throw system_error();
}

inline
Socket::Socket(int f, Transport t, const Address& l, const Address& p)
  : transport(t), local(l), peer(p), fd(f)
{ }

inline
Socket::Socket(Socket&& s)
  : local(std::move(s.local)), peer(std::move(s.peer))
{
  fd = s.fd;
  s.fd = -1;
}

inline
Socket::~Socket() { 
  if (fd >= 0)
    ::close(fd); 
}

inline Error
bind(Socket& s, Address a) {
  s.local = a;
  auto result = ::bind(s.fd, addr(s.local), len(s.local));
  if(result != 0)
    return system_error();
  return {};
}

inline Error
connect(Socket& s, const Address& a) {
  s.peer = a;
  auto result = ::connect(s.fd, addr(s.peer), len(s.peer));
  if(result != 0)
    return system_error();
  return {};
}

inline Error
listen(Socket& s, int backlog) {
  s.backlog = backlog;
  auto result = ::listen(s.fd, backlog);
  if(result != 0)
    return system_error();
  return {};
}

inline Socket
accept(Socket& s) {
  socklen_t length;
  int child = ::accept(s.fd, addr(s.peer), &length);
  if(child < 0)
    throw system_error();
  return Socket(child, s.transport, s.local, s.peer);
}

inline int
read(Socket& s, uint8_t* b, std::size_t l) {
  auto result = ::read(s.fd, b, l);
  if(result < 0)
    throw system_error();
  return result;
}

inline int
write(Socket& s, const uint8_t* b, std::size_t l) {
  auto result = ::write(s.fd, b, l);
  if(result < 0)
    throw system_error();
  return result;
}

inline int
recvfrom(Socket& s, uint8_t* b, std::size_t l, Address& a) {
  socklen_t length;
  auto result = ::recvfrom(s.fd, b, l, 0, addr(a), &length);
  if(result < 0)
    throw system_error();
  return result;
}

inline int
sendto(Socket& s, const uint8_t* b, std::size_t l, const Address& a) {
  auto result = ::sendto(s.fd, b, l, 0, addr(a), len(a));
  if(result < 0)
    throw system_error();
  return result;
}

} // namespace socket
} // namespace freeflow
