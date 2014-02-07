#include <signal.h>
#include <iostream>

void 
handler(int v)
{
  std::cout << "blah" << std::endl;
  std::exit(-1);
}

int 
main(int argc, char** argv)
{
  ::signal(SIGINT, handler);
  while(true) {}
}
