#include <iostream>
#include <string.h>
#include <unistd.h>

#include "freeflow/sys/selector.hpp"

using namespace freeflow;

int 
main(int argc, char* argv[]) {
  
  // Create a selector and add stdin as a potential reader.
  // FIXME: Integrate this with Resource interfaces.
  Selector s;
  s.add_reader(0); // 0 == fileno(stdin)

  while(true) {
    std::cout << ">" << std::flush;
    
    // Wait to see if any events are available.
    Microseconds ms(1000);
    s(ms);

    // TODO: Rewrite this to use the buffer class.
    if(s.is_readable(0)) {
      char buf[256];
      ::memset(buf, 0, 256);
      int n = ::read(0, buf, 256);
      if (n == -1)
        break;
      std::cout << buf << '\n';
    }
  }
}
