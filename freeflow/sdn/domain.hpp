// Copyright (c) 2013-2014 Flowgrammable, LLC.
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

#ifndef FREEFLOW_DOMAIN_HPP
#define FREEFLOW_DOMAIN_HPP

#include <functional>
#include <vector>
#include <string>
#include <unordered_map>

#include <freeflow/sys/socket.hpp>
#include <freeflow/sys/acceptor.hpp>

namespace freeflow {

class Listener;

/// The Domain class represents a logical grouping of resources, for
/// example: an entire network, regional subsets of a networks, or
/// functionaly isolated networks. Grouping resources into domains
/// makes it possible to apply policies or launch applicaitons across
/// a broad set of assets.
///
/// In this design, networks are represented as sets of connected
/// switches, each connected to the controller. Each domain contains
/// a set of configurabl control ports, sockets to which a switch
/// can connect.
class Domain {
  using Listener_list = std::vector<Listener*>;
public:
  struct Hash;

  Domain(const std::string&);

  // Accessors
  const std::string& name() const;

  // Port management
  Listener* add_listener(const Address& a, Socket::Transport, int);
  void remove_listener(Listener*);

private:
  std::string   name_;
  Listener_list port_;
};


/// A hashing function for Domain objects. This is the same as the
/// hashh of the Domain's name.
struct Domain::Hash {
  std::size_t operator()(const Domain&) const;
};


/// A Domain_map uniquely associates domains with their name.
using Domain_map = std::unordered_map<std::string, Domain, Domain::Hash>;


/// The domain manager maintains a set of unordered names where
/// each domain is uniquely identified by its name.
class Domain_manager : public Domain_map
{
  using Domain_map::Domain_map;

  // FIXME: What goes here?
};

} // namespace freeflow

#include <freeflow/sdn/domain.ipp>

#endif

