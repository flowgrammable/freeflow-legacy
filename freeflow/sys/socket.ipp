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

inline
Address::Address()
{
  ::memset(&storage, 0, sizeof(sockaddr_storage));
  storage.ss_family = IPv4;
}

inline
Address::Address(Type t, const std::string n, uint16_t p)
{
  ::memset(&storage, 0, sizeof(sockaddr_storage));
  storage.ss_family = t;
  if(t == IPv4) {
    sockaddr_in *v4 = reinterpret_cast<sockaddr_in*>(&storage);
    v4->sin_port = htons(p);
    if (inet_pton(IPv4, n.c_str(), &v4->sin_addr) != 1)
      throw Error(Error::SYSTEM_ERROR, errno);
  } else if(t == IPv6) {
    sockaddr_in6 *v6 = reinterpret_cast<sockaddr_in6*>(&storage);
    v6->sin6_port = htons(p);
    if (inet_pton(IPv6, n.c_str(), &v6->sin6_addr) != 1)
      throw Error(Error::SYSTEM_ERROR, errno);
  }
}

inline
Address::Address(ipv4::Address a, uint16_t p)
{
  ::memset(&storage, 0, sizeof(sockaddr_storage));
  sockaddr_in *v4 = reinterpret_cast<sockaddr_in*>(&storage);

  v4->sin_family = IPv4;
  v4->sin_port = htons(p);
  ::memcpy(&v4->sin_addr, &a.value, sizeof(in_addr));
}

inline
Address::Address(ipv6::Address a, uint16_t p)
{
  ::memset(&storage, 0, sizeof(sockaddr_storage));
  sockaddr_in6 *v6 = reinterpret_cast<sockaddr_in6*>(&storage);

  v6->sin6_family = IPv6;
  v6->sin6_port = htons(p);
  ::memcpy(&v6->sin6_addr, &a.value, sizeof(in6_addr));
}

inline
Address::Address(const Address& a)
{
  memcpy(&storage, &a.storage, sizeof(sockaddr_storage));
}

inline Address&
Address::operator=(const Address& a)
{
  memcpy(&storage, &a.storage, sizeof(sockaddr_storage));
  return *this;
}

inline bool
operator==(const Address& l, const Address& r)
{
  if(l.storage.ss_family != r.storage.ss_family)
    return false; 
  if(l.storage.ss_family == Address::IPv4) {
    const sockaddr_in *lv4 = reinterpret_cast<const sockaddr_in*>(&l.storage);
    const sockaddr_in *rv4 = reinterpret_cast<const sockaddr_in*>(&r.storage);
    if(lv4->sin_port != rv4->sin_port)
      return false;
    if(memcmp(&lv4->sin_addr, &rv4->sin_addr, sizeof(in_addr)) == 0)
      return true;
  } else if (l.storage.ss_family == Address::IPv6) {
    const sockaddr_in6 *lv6 = reinterpret_cast<const sockaddr_in6*>(&l.storage);
    const sockaddr_in6 *rv6 = reinterpret_cast<const sockaddr_in6*>(&r.storage);
    if(lv6->sin6_port != rv6->sin6_port)
      return false;
    if(memcmp(&lv6->sin6_addr, &rv6->sin6_addr, sizeof(in6_addr)) == 0)
      return true;
  }
  return false;
}

inline bool
operator!=(const Address& l, const Address& r)
{
  return not (l==r);
}

inline sa_family_t
family(const Address& a)
{
  return a.storage.ss_family;
}

inline sockaddr*
addr(Address& a)
{
  return reinterpret_cast<sockaddr*>(&a.storage);
}

inline socklen_t
len(const Address& a)
{
  return sizeof(sockaddr);
}

inline
Socket::Socket(int f, Transport t, const Address& l, const Address& p)
  : local(l), peer(p), transport(t), fd(f)
{ }

inline
Socket::Socket(Transport t, Address a)
  : local(a), transport(t)
{
  fd = ::socket(family(local), transport, 0);
  if (fd < 0)
    throw Error(Error::SYSTEM_ERROR, errno);
}

inline
Socket::Socket(Socket&& s)
{
  // Move the address(s)
  local = std::move(s.local);
  peer  = std::move(s.peer);

  // Move the fd
  fd = s.fd;
  s.fd = -1;
}

inline
Socket::~Socket()
{
  close(*this);
}

inline Error
bind(Socket& s, Address a)
{
  s.local = a;
  std::cout << "binding: " << to_string(s.local) << std::endl;
  auto result = ::bind(s.fd, addr(s.local), len(s.local));
  if(result != 0)
    return Error(Error::SYSTEM_ERROR, errno);
  else
    return Error();
}

inline Error
connect(Socket& s, const Address& a)
{
  s.peer = a;
  auto result = ::connect(s.fd, addr(s.peer), len(s.peer));
  if(result != 0)
    return Error(Error::SYSTEM_ERROR, errno);
  else
    return Error();
}

inline Error
listen(Socket& s, int backlog)
{
  s.backlog = backlog;
  auto result = ::listen(s.fd, backlog);
  if(result != 0)
    return Error(Error::SYSTEM_ERROR, errno);
  else
    return Error();
}

inline Socket
accept(Socket& s)
{
  socklen_t length;
  int child = accept(s.fd, addr(s.peer), &length);
  if(child < 0)
    throw Error(Error::SYSTEM_ERROR, errno);
  return Socket(child, s.transport, s.local, s.peer);
}

inline int
read(Socket& s, uint8_t* b, std::size_t l)
{
  auto result = ::read(s.fd, b, l);
  if(result < 0)
    throw Error(Error::SYSTEM_ERROR, errno);
  return result;
}

inline int
write(Socket& s, const uint8_t* b, std::size_t l)
{
  auto result = ::write(s.fd, b, l);
  if(result < 0)
    throw Error(Error::SYSTEM_ERROR, errno);
  return result;
}

inline int
recvfrom(Socket& s, uint8_t* b, std::size_t l, Address& a)
{
  socklen_t length;
  auto result = ::recvfrom(s.fd, b, l, 0, addr(a), &length);
  if(result < 0)
    throw Error(Error::SYSTEM_ERROR, errno);
  return result;
}

inline int
sendto(Socket& s, const uint8_t* b, std::size_t l, const Address& a)
{
  auto result = ::sendto(s.fd, b, l, 0, addr(a), len(a));
  if(result < 0)
    throw Error(Error::SYSTEM_ERROR, errno);
  return result;
}


inline Error
close(Socket& s)
{
  if(s.fd > -1) {
    auto result = ::close(s.fd);
    s.fd = -1;
    if(result == -1) 
      return Error(Error::SYSTEM_ERROR, errno);
  }
  return Error();
}

} // namespace socket
} // namespace freeflow
