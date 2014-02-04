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

#include <sstream>
#include "socket.hpp"

namespace freeflow {
namespace socket {

std::string
to_string(const Address& a)
{
  std::stringstream ss;
  if(a.type == Address::IPv4) {
    char name[INET_ADDRSTRLEN];
    ::bzero(&name, INET_ADDRSTRLEN);
    inet_ntop(Address::IPv4, &a.v4.sin_addr, name, sizeof(in_addr));

    ss << "ipv4(";
    ss << name;
    ss << ",";
    ss << ntohs(a.v4.sin_port) ;
    ss << ")";
  } else if(a.type == Address::IPv6) {
    char name[INET6_ADDRSTRLEN];
    ::bzero(&name, INET6_ADDRSTRLEN);
    inet_ntop(Address::IPv6, &a.v6.sin6_addr, name, sizeof(in6_addr));

    ss << "ipv6(";
    ss << name;
    ss << ",";
    ss << ntohs(a.v6.sin6_port) ;
    ss << ")";
  }

  return ss.str();
}

std::string
to_string(const Socket& s)
{
  std::stringstream ss;
  ss << "socket(";
  ss << "local=" << to_string(s.local);
  ss << ",";
  ss << "peer=" << to_string(s.peer);
  ss << ")";
  return ss.str();
}

} // namespace socket
} // namespace freeflow

