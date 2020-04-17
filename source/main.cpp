#include "version_lib.h"
#include <iostream>
#include <csignal>
#include "data.h"


int countBulk;
void checkArg(int, char**);
DataIn* bulkPtr;
void signalFunction(int sig)
{
  std::cout << std::endl;
  if (sig == 2)
  {
    if (bulkPtr->getQuantity()) bulkPtr->notify();
  }
  exit(0);
}

int main(int argc, char *argv[]) 
{
  try
  {
    checkArg(argc,argv);

    DataIn bulk(countBulk);
    bulkPtr = &bulk;

    DataToConsole console(bulkPtr);
    DataToFile file(bulkPtr);
    
    signal(SIGINT,signalFunction);
    
    std::string str;
    while(getline(std::cin,str))
    {
      bulk.setData(std::move(str));
    }

  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

    return 0;
}
