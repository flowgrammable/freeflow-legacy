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
#include <sstream>

#include "socket.hpp"

namespace freeflow {

const Ipv4_addr Ipv4_addr::any(htonl(INADDR_ANY));
const Ipv4_addr Ipv4_addr::broadcast(htonl(INADDR_BROADCAST));
const Ipv4_addr Ipv4_addr::loopback(htonl(INADDR_LOOPBACK));

const Ipv6_addr Ipv6_addr::any(in6addr_any);
const Ipv6_addr Ipv6_addr::loopback(in6addr_loopback);

/// Write the IPv4 address to the given stream. The failbit is set if
/// the address cannot be rendered.
///
/// \todo Under what circumstances could the address be invalid?
std::ostream&
operator<<(std::ostream& os, const Ipv4_addr& a) {
  constexpr socklen_t N = 16; // 4*3 digits + 3 separators + padding
  char buf[N];
  if (not inet_ntop(Address_info::IP4, &a, buf, N))
    os.setstate(std::ios::failbit);
  else
    os << buf;
  return os;
}

/// Write the IPv6 address to the given stream. The failbit is set if
/// the address cannot be rendered.
///
/// \todo Under what circumstances could the address be invalid?
std::ostream&
operator<<(std::ostream& os, const Ipv6_addr& a) {
  constexpr socklen_t N = 48; // 8*4 digits + 7 separators + padding
  char buf[N];
  if (not inet_ntop(Address_info::IP6, &a, buf, N))
    os.setstate(std::ios::failbit);
  else
    os << buf;
  return os;
}


std::string
to_string(const Address& a) {
  std::stringstream ss;
  if(a.family() == Address::IP4) {
    char name[INET_ADDRSTRLEN];
    ::memset(&name, 0, INET_ADDRSTRLEN);
    const sockaddr_in *v4 = reinterpret_cast<const sockaddr_in*>(&a.storage);
    inet_ntop(Address::IP4, &v4->sin_addr, name, sizeof(in_addr));

    ss << "ipv4(";
    ss << name;
    ss << ",";
    ss << ntohs(v4->sin_port) ;
    ss << ")";
  } else if(a.family() == Address::IP6) {
    char name[INET6_ADDRSTRLEN];
    ::memset(&name, 0, INET6_ADDRSTRLEN);
    const sockaddr_in6 *v6 = reinterpret_cast<const sockaddr_in6*>(&a.storage);
    inet_ntop(Address::IP6, &v6->sin6_addr, name, sizeof(in6_addr));

    ss << "ipv6(";
    ss << name;
    ss << ",";
    ss << ntohs(v6->sin6_port) ;
    ss << ")";
  }

  return ss.str();
}

std::string
to_string(const Socket& s) {
  std::stringstream ss;
  ss << "socket(";
  ss << "local=" << to_string(s.local);
  ss << ",";
  ss << "peer=" << to_string(s.peer);
  ss << ")";
  return ss.str();
}

} // namespace freeflow

