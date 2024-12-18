#include <iostream>
#include <vector>
#include "errorHandle.h"
#include "scanner.h"
#include "parser.h"
#include "walker.h"
int main(int argc, char** argv){
    if(argc!=2)
        error("Correct Syntax ./main <filename>");
    loadKeywords();

    std::vector<Token> tokens = Scanner(argv[1]);
    Parser parser(tokens);
    Node* root = parser.parseProgram();
   mainWalker(root);

}
