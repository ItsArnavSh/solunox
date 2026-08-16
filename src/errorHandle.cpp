#include <cstdlib>
#include <iostream>
#include "errorHandle.h"
void error(std::string err){
    std::cout << err<<std::endl;
    exit(EXIT_FAILURE);
}
