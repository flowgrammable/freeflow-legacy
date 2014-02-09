#include <string.h>
#include <iostream>
#include <freeflow/sys/kqueue.hpp>

int 
main(int argc, char** argv)
{
  using namespace freeflow;

  KQueue kq;
  add_timer(kq, 0, MicroTime(100000));

  try {

    bool done = false;
    int cnt = 0;
    while(not done) {
      auto events = kevent(kq, MicroTime(200000));
      for(auto event : events) {
        if(cnt == 20) {
          del_timer(kq, 0);
        } else {
          ++cnt;
        }
        std::cout << "tick: " << cnt << std::endl;
      }
      if(events.size() == 0)
        done = true;
  }
  std::cout << "complete" << std::endl;

  } catch(Error e) {
    std::cout << strerror(e.data()) << std::endl;
  }
}
