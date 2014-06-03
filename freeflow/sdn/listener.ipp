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

namespace freeflow {

template<typename S>
  Default_listener_factory<S>::Default_listener_factory(Controller& c)
    : ctrl_(c) { }

template<typename S>
  inline S*
  Default_listener_factory<S>::operator()(Reactor& r, Socket&& s) const {
    return new S(r, std::move(s), ctrl_);
  }

template<typename S, typename F>
  Listener<S, F>::Listener(Controller& c)
    : Acceptor<S, F>(c, c) { }

} // namespace freeflow
