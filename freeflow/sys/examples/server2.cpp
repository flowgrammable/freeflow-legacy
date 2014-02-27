
#include <cstdlib>
#include <iostream>

#include <freeflow/sys/socket.hpp>

int main(int argc, char** argv) {
  if(argc != 5) {
    std::cerr << "usage syntax: " << argv[0] << "ipv4|ipv6 addr port tcp|udp" << std::endl;
    std::exit(-1);
  }

  std::string ipv4("ipv4");
  std::string ipv6("ipv6");
  std::string tcp("tcp");
  std::string udp("udp");

  using namespace freeflow;

  Socket::Transport t;
  if (tcp.compare(argv[4]) == 0)
    t = Socket::TCP;
  else if (tcp.compare(argv[4]) == 0)
    t = Socket::UDP;

  Address::Family net;
  if (ipv4.compare(argv[1]) == 0)
    net = Address::IPv4;
  else if (ipv4.compare(argv[1]) == 0)
    net = Address::IPv6;

  uint16_t port = atoi(argv[3]);
  
  // try {
    Socket server(net, t);
    bind(server, Address(net, argv[2], port));
    if (t == Socket::TCP) {
      listen(server);
      std::cout << "Listening: " << to_string(server.local) << std::endl ;
      Socket child = accept(server);
    } else if (t == Socket::UDP) {
    }
  // } catch(Error e) {
  //   std::cerr << "Error: " << strerror(e.code()) << std::endl;
  // }
}
