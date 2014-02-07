
#include <string.h>
#include <signal.h>

#include <iostream>

#include "freeflow/sys/fd_utils.hpp"
#include "freeflow/sys/pipe.hpp"
#include "freeflow/sys/scheduler.hpp"

struct Console : public freeflow::Task
{
  Console(const std::string& n) :
    Task(READABLE, 1), pipe(n)
  { }

  void init(const freeflow::TimePoint& tp) {
    std::cout << ">" << std::flush;
  }

  void read(const freeflow::TimePoint& tp) {
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
    std::cout << ">" << std::flush;
  }

  int fd() const { return pipe.fd; }

  freeflow::Pipe pipe;
};

freeflow::Scheduler sched;

void 
handler(int arg)
{
  freeflow::clr_tasks(sched);
}

int 
main(int argc, char** argv)
{
  if(argc != 2) {
    std::cerr << "usage error: " << argv[0] << " <pipe>" << std::endl;
    std::exit(-1);
  }

  using namespace freeflow;
  ::signal(SIGINT, handler);
  try {
    Console console(argv[1]);
    add_task(sched, &console);
    run(sched);
  } catch(Error e) {
    std::cerr << strerror(e.data()) << std::endl;
  }

}
