
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "freeflow/sys/scheduler.hpp"

struct Console : public freeflow::Task
{
  Console() : Task(READABLE, 1)
  { }

  void init(const freeflow::TimePoint& tp) {
    std::cout << ">" << std::flush;
  }

  void read(const freeflow::TimePoint& tp) {
    char input[256];
    ::memset(input, 0, 256);
    ::read(fileno(stdin), input, 256);
    std::cout << ">" << std::flush ;
  }

  int fd() const { return fileno(stdin); }
};

int main() {
  using namespace freeflow;

  Console console;
  Scheduler scheduler;

  add_task(scheduler, &console);
  run(scheduler);
}
