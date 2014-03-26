
#include <cstdlib>
#include <iostream>

#include <freeflow/sys/socket.hpp>

using namespace freeflow;

int main(int argc, char** argv) {
  if(argc != 5) {
    std::cerr << "usage syntax: " << argv[0] 
              << "ipv4|ipv6 addr port tcp|udp" << std::endl;
    std::exit(-1);
  }

  std::string ipv4 = "ipv4";
  std::string ipv6 = "ipv6";
  std::string tcp = "tcp";
  std::string udp = "udp";

  // Select the address family.
  Address::Family net;
  if (argv[1] == ipv4)
    net = Address::IP4;
  else if (argv[1] == ipv6)
    net = Address::IP6;

  // Get the addres and port.
  std::string addr = argv[2];
  Ip_port port = atoi(argv[3]);

  // Select the transpot
  Socket::Transport t;
  if (argv[4] == tcp)
    t = Socket::TCP;
  else if (argv[4] == udp)
    t = Socket::UDP;


  try {
    Socket server(net, t);
    server.bind(Address(net, addr, port));
    if (t == Socket::TCP) {
      server.listen();
      std::cout << "Listening: " << to_string(server.local) << std::endl ;
      Socket child = server.accept();
    } else if (t == Socket::UDP) {
    }
  } catch(Error e) {
    ::perror("Error");
  }
}
