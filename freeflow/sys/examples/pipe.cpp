#include <string.h>
#include <iostream>
#include "freeflow/sys/fd_utils.hpp"
#include "freeflow/sys/pipe.hpp"

int main(int argc, char** argv) {
  if(argc != 2) {
    std::cerr << "usage error: " << argv[0] << " <pipe>" << std::endl;
    std::exit(-1);
  }

  try {
    freeflow::Pipe pipe(argv[1]);
    char input[256];
    ::memset(input, 0, 256);
    freeflow::Transfer t(pipe.fd, (uint8_t*)input, 256);
    auto result = freeflow::read_available(t);
    if(result < 0) {
      std::cerr << "Error: " << strerror(errno) << std::endl;
    } else {
      std::cout << "Rx Bytes: " << result << std::endl;
      std::cout << input << std::endl;
    }
  } catch(freeflow::Error e) {
    std::cerr << strerror(e.data()) << std::endl;
  }

}
