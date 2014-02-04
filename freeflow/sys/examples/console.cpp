#include <iostream>

bool
eval_bind()
{
  return false;
}

bool
eval_help()
{
  std::cout << std::string(40, '-') << std::endl;
  std::cout << "bind" << "\t" << "create and bind a socket" << std::endl;
  std::cout << "quit" << "\t" << "terminate the console" << std::endl;
  std::cout << "help" << "\t" << "print a listing of commands" << std::endl;
  std::cout << std::string(40, '-') << std::endl;
  return false;
}

bool
process(const std::string& input)
{
  std::string bind("bind");
  std::string help("help");
  std::string quit("quit");

  if(input.compare(bind) == 0)
    return eval_bind();
  else if(input.compare(help) == 0)
    return eval_help();
  else if(input.compare(quit) == 0)
    return true;
  return false;
}

int 
main(int argc, char** argv)
{
  std::string input;
  bool done = false;
  while(not done) {
    std::cout << ">";
    std::getline(std::cin, input);
    done = process(input);
  }
}
