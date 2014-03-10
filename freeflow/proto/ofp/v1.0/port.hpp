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

#ifndef FREEFLOW_OFPV1_0_PORT_HPP
#define FREEFLOW_OFPV1_0_PORT_HPP

#include <freeflow/sys/error.hpp>
#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/ofp.hpp>
#include <freeflow/proto/ofp/v1.0/error.hpp>

namespace freeflow {
namespace ofp {
namespace v1_0 {

/// Represents a physical or logical port on a switch. The Port
/// class contains configuration information that describes a physical
/// or logical port. This includes its id, Ethernet address, configuration,
/// state, and supported features.
struct Port {
  /// Identifies the port number. The Id is a 16-bit integer value 
  /// representing a physical port. There are a number of special ports 
  /// indicated by named values.
  using Id = Uint16;
  static constexpr Id MAX         = 0xff00;
  static constexpr Id IN_PORT     = 0xfff8;
  static constexpr Id TABLE       = 0xfff9;
  static constexpr Id NORMAL      = 0xfffa;
  static constexpr Id FLOOD       = 0xfffb;
  static constexpr Id ALL         = 0xfffc;
  static constexpr Id CONTROLLER  = 0xfffd;
  static constexpr Id LOCAL       = 0xfffe;
  static constexpr Id NONE        = 0xffff;

  /// Describes the configuration of a port.
  enum Config : Uint32 {
    PORT_DOWN    = 0x00000001, 
    NO_STP       = 0x00000002, 
    NO_RECV      = 0x00000004,
    NO_RECV_STP  = 0x00000008, 
    NO_FLOOD     = 0x00000010, 
    NO_FWD       = 0x00000020,
    NO_PACKET_IN = 0x00000040
  };

  /// Describes the current state of a port.
  enum State : Uint32 {
    STP_LISTEN  = 0x00000000, 
    LINK_DOWN   = 0x00000001, 
    STP_LEARN   = 0x00000002,
    STP_FORWARD = 0x00000004, 
    STP_BLOCK   = 0x00000008, 
    STP_MASK    = 0x00000010
  };  

  /// Describes the features of a port.
  enum Features : Uint32 {
    TEN_MB_HD     = 0x0000001, 
    TEN_MB_FD     = 0x00000002, 
    HUNDRED_MB_HD = 0x00000004, 
    HUNDRED_MB_FD = 0x00000008, 
    ONE_GB_HD     = 0x00000010, 
    ONE_GB_FD     = 0x00000020,
    TEN_GB_FD     = 0x00000040, 
    COPPER        = 0x00000080, 
    FIBER         = 0x00000100,
    AUTO_NEG      = 0x00000200, 
    PAUSE         = 0x00000400, 
    PAUSE_ASYM    = 0x00000800
  };

  Id         port_id;
  Mac_addr   hw_addr;
  String<16> name;
  Config     config;
  State      state;
  Features   current;
  Features   advertised;
  Features   supported;
  Features   peer;
};

// Protocol
constexpr std::size_t bytes(const Port&);
Errc to_view(View&, const Port&);
Errc from_view(View&, Port&);

} // namespace v1_0
} // namespace ofp
} // namespace freeflow

#include "port.ipp"

#endif