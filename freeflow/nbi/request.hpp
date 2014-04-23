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

#ifndef FREEFLOW_REQUEST_HPP
#define FREEFLOW_REQUEST_HPP

#include <queue>

namespace freeflow {

/// Represents a request to disconnect the switch.
struct Disconnect_request { };

/// Represents a request to terminate the application.
struct Terminate_request { };

/// The Request class represents a request from an application to
/// the controller.
struct Request {
  enum Type {
    DISCONNECT,
    TERMINATE
  };

  union Data {
    Disconnect_request disconnect;
    Terminate_request terminate;
  };

  Type type;
  Data data;
};

/// The request queue stores requests from the application.
using Request_queue = std::queue<Request>;

} // namespace freeflow

#include <freeflow/nbi/request.ipp>

#endif

