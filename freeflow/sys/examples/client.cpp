#include <iostream>
#include <string.h>
#include <unistd.h>

#include "freeflow/sys/selector.hpp"

int main(int argc, char** argv) {
  std::cout << "Client..." << std::endl;

  using namespace freeflow;
  Selector sel;

  bool done = false;
  while(not done) {
    add_reader(sel, fileno(stdin));
    std::cout << ">" << std::flush ;

    auto result = select(sel, nullptr);

    char input[256];
    ::memset(input, 0, 256);

    if(is_readable(sel, fileno(stdin))) {
      read(fileno(stdin), input, 256);
      std::cout << "input was: " << input ;
    }

  }
}
