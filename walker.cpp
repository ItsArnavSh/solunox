#include "walker.h"
#include "containers.h"
#include "token.h"
std::stack<containers*> functionStack;
void mainWalker(Node *nt){
    if(mainLocation==-1){
        error("No Main Function found");
    }
    solveFunction(nt->children[0]->children[mainLocation]);
}
void solveFunction(Node* start){
    containers *func = new containers(STACK,STACK,STACK);
    functionStack.push(func);
    //start[1] is declare, do it later
    //start[2] is statements
    solveStatements(start->children[1]);
}
void solveStatements(Node* start){
    for(auto *statement : start->children){
        solveStatements(statement);
    }
}
void solveIF(Node* start){}
void solveLoop(Node* start){}
void solveStatement(Node* start){
    switch(start->type){
        case LEFTPUSH:

    }
}

int eval(){

}
