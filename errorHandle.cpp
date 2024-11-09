#include <cstdlib>
#include <iostream>
#include "error.h"
void error(std::string err){
    std::cout << err<<std::endl;
    exit(EXIT_FAILURE);
}
