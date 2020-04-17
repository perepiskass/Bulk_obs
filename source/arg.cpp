#include "version_lib.h"
#include <iostream>
#include <string>

/**
 * @brief Функция для проверки введенных аргументов программы.
 */
void checkArg(int argc,char** argv)
{
      extern int countBulk;

    if ( argc > 1)
    {
        std::string version = "-version";
        if (argv[1] == version)
        {
            std::cout << "version: " << version_major()<< '.'<< version_minor() << '.' << version_patch() << std::endl;
            exit(0);
        }
        else
        {
            auto at = atoi(argv[1]);
            if (at > 0)  countBulk = at;
            else  
            {

                std::cout << "Enter correct bulk count"<< std::endl;
                exit(0);
            }
        }
    }
    else 
    {
      std::cout << "Enter bulk count or -version" << std::endl;
      exit(0);
    }
    
}