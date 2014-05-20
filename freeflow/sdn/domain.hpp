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

#include <string>

namespace freeflow {

struct Controller_port;

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
  Domain(const std::string&);

  const std::string& name() const;

private:
  std::string name_;
};

} // namespace freeflow

#endif

