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

namespace freeflow {
namespace socket {

inline
Address::Address(Type t, const std::string n) : 
  type(t)
{
  ::bzero(&v4, sizeof(sockaddr));
  if(t == IPv4) {
    v4.sin_family = IPv4;
    if (inet_pton(IPv4, n.c_str(), &v4.sin_addr) != 1)
      throw 1;
  } else if(t == IPv6) {
    v6.sin6_family = IPv6;
    if (inet_pton(IPv6, n.c_str(), &v6.sin6_addr) != 1)
      throw 1;
  }
}

inline
Address::Address(ipv4::Address a, uint16_t p)
{
  ::bzero(&v4, sizeof(sockaddr));
  v4.sin_family = IPv4;
  ::memcpy(&v4.sin_addr, &a.value, sizeof(in_addr));
  v4.sin_port = htons(p);
}

inline
Address::Address(ipv6::Address a, uint16_t p)
{
  ::bzero(&v6, sizeof(sockaddr));
  v6.sin6_family = IPv6;
  ::memcpy(&v6.sin6_addr, &a.value, sizeof(in6_addr));
  v6.sin6_port = htons(p);
}

inline
Socket::~Socket()
{
  if(fd > -1)
    ::close(fd);
}

} // namespace socket
} // namespace freeflow
