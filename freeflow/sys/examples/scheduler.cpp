
#include <unistd.h>
#include <string.h>
#include <iostream>

#include <freeflow/sys/scheduler.hpp>

using namespace freeflow;

struct Console : public freeflow::Task {
  Console() : Task(READABLE, 1)
  { }

  void init(freeflow::Time_point tp) {
    std::cout << ">" << std::flush;
  }

  void read(freeflow::Time_point tp) {
    char input[256];
    ::memset(input, 0, 256);
    ::read(fileno(stdin), input, 256);
    std::cout << ">" << std::flush ;
  }

  int fd() const { return fileno(stdin); }
};

int
main() {
  Console c;

  Scheduler s;
  s.add(&c);
  s();
}
