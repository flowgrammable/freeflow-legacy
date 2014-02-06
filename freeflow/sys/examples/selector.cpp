#include <iostream>
#include <string.h>
#include <unistd.h>

#include "freeflow/sys/selector.hpp"

int main(int argc, char** argv) {

  using namespace freeflow;
  Selector sel;
  add_reader(sel, fileno(stdin));

  while(true) {
    std::cout << ">" << std::flush;
    auto result = select(sel, nullptr);

    if(is_readable(sel, fileno(stdin))) {
      char input[256];
      ::memset(input, 0, 256);
      read(fileno(stdin), input, 256);
    }
  }
}
