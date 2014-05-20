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

class Application;

/// Enumerates the types of application requests.
enum class Request_type {
  DISCONNECT,
  TERMINATE
};

/// Represents a request to disconnect the switch.
struct Disconnect_request { 
  static constexpr Request_type Kind = Request_type::DISCONNECT;
};

/// Represents a request to terminate the application.
struct Terminate_request { 
  static constexpr Request_type Kind = Request_type::TERMINATE;
};


/// The request value is a union of different request types.
union Request_data {
  Request_data(const Disconnect_request&);
  Request_data(const Terminate_request&);

  Disconnect_request disconnect;
  Terminate_request terminate;
};


/// The Request class represents a request from an application to
/// the controller.
struct Request {
  using Type = Request_type;
  static constexpr Type DISCONNECT = Type::DISCONNECT;
  static constexpr Type TERMINATE = Type::TERMINATE;

  using Data = Request_data;

  template<typename T>
    Request(Application*, const T&);

  Application* app;   // The requesting pplication
  Type         type;  // The request type
  Data         data;  // Additional request data
};

/// The request queue stores requests from the application.
using Request_queue = std::queue<Request>;

} // namespace freeflow

#include <freeflow/sdn/request.ipp>

#endif

