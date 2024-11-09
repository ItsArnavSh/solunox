#include <iostream>
#include <vector>
#include "errorHandle.h"
#include "scanner.h"
int main(int argc, char** argv){
    if(argc!=2)
        error("Correct Syntax ./main <filename>");
    std::vector<Token> tokens = Scanner(argv[1]);
    for(auto token:tokens){
        token.printToken();
    }
}
