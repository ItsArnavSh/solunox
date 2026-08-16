#include "walker.h"
#include "containers.h"
#include "token.h"
#include <cstddef>
#include <iostream>
#include <stack>
std::stack<bool> loopFlags;//For Keeping track of break statements in nested if-loops
std::stack<bool> functionFlags;
std::stack<containers> functionStack;
std::stack<int> omnis;
Node* root;
void mainWalker(Node *nt){
    root = nt;
    if(mainLocation==-1){
        error("No Main Function found");
    }
    solveFunction(nt->children[mainLocation]);
    std::cout << std::endl;
}
TokenType DivideAndConquor(short num){
    switch(num){
        case 0:
            return STACK;
        case 1:
            return QUEUE;
        case 2:
            return PRIORITYQUEUE;
        default:
            error("Error in validation");
            return FUNCTION;
    }
}
void solveFunction(Node* start){
    if(start->children.size()==0){
        error("Function Not Found");
    }
    functionFlags.push(false);

    int num = start->children[0]->value.number;
    //std::cout << num<<std::endl;
    //mod 3 once -> nox
    short n = num%3;
    num = num/3;
    short l = num%3;
    num = num/3;
    short s = num%3;
    containers func(DivideAndConquor(s),DivideAndConquor(l),DivideAndConquor(n));
    functionStack.push(func);
    //start[2] is statements
    solveStatements(start->children[1]);
    functionStack.pop();//Remove Function from stack after operation
    functionFlags.pop();
}
void solveStatements(Node* start){
    for(auto *statement : start->children){
        solveAnyStatement(statement);
        if(functionFlags.top())
            return;

    }
}
void solveAnyStatement(Node* statement) {
    switch (statement->type) {
        case CALLER:
            solveFunction(root->children[statement->value.identifierKey]);
            break;
        case LOOP:
            solveLoop(statement->children[0]);
            break;
        case IF:
            solveIF(statement);
            break;
        case BREAK:
            if(loopFlags.empty())
                error("No loops for break statement");
            loopFlags.top() = true;
            break;
        case RETURN:
            functionFlags.top()=true;
            return;
        default:
            solveStatement(statement);  // Handle other statements like assignments
            break;
    }
}
void solveIF(Node* start){
    for(auto condition:start->children){
        if(eval(condition->children[0])){

            solveStatements(condition->children[1]);
            for(auto *statement : condition->children[1]->children){
                solveAnyStatement(condition);
                if(functionFlags.top())
                    return;
            }
            break;
        }
    }
}
void solveLoop(Node* start){
    loopFlags.push(false);
    while(true){
        for(auto *statement : start->children){
            solveAnyStatement(statement);
            if(functionFlags.top())
                return;
            if(loopFlags.top())
                break;
            }
            if(loopFlags.top())
                break;
        }
        loopFlags.pop();
    }

void popper(Node* start){
    switch(start->children[1]->type){
        case SOL:
        functionStack.top().sol.pop();
        break;
        case LUNA:
        functionStack.top().luna.pop();
        break;
        case NOX:
        functionStack.top().nox.pop();
        case OMNIS:
        omnis.pop();
        default:
        return;

    }
}
void solveStatement(Node* start){
    Node* leftOperand = start->children[0];
    for(int i=1;i<start->children.size();i++){

    switch(start->type){
        case EQUAL:
        switch(start->children[0]->type){
        case SOL:
        functionStack.top().sol.remove();
        functionStack.top().sol.add(eval(start->children[i]));
        break;
        case LUNA:
        functionStack.top().luna.remove();
        functionStack.top().luna.add(eval(start->children[i]));
        break;
        case NOX:
        functionStack.top().luna.remove();
        functionStack.top().nox.add(eval(start->children[i]));
        break;
        case PRINT:
        std::cout<<eval(start->children[i])<<std::endl;
        popper(start);
        break;
        case PRINTC:
        std::cout<<char(eval(start->children[i])%128);
        popper(start);
        break;
        case OMNIS:
        functionStack.top().sol.remove();
        omnis.push(eval(start->children[i]));
        break;
        default:
        error("Container or Print Expected");
        }
        break;
        case LEFTPUSH:
        switch(start->children[0]->type){
        case SOL:
        functionStack.top().sol.add(eval(start->children[i]));
        popper(start);
        break;
        case LUNA:
        functionStack.top().luna.add(eval(start->children[i]));
        popper(start);
        break;
        case NOX:
        functionStack.top().nox.add(eval(start->children[i]));
        popper(start);
        break;
        case PRINT:
        std::cout<<eval(start->children[i])<<std::endl;
        popper(start);
        break;
        case PRINTC:
        std::cout<<char(eval(start->children[i])%128);
        popper(start);
        break;
        case OMNIS:
        omnis.push(eval(start->children[i]));
        popper(start);
        break;
        default:
        error("Container or Print Expected");
        }
        break;
        case LEFTCOPY:
        switch(start->children[0]->type){
        case SOL:
        functionStack.top().sol.add(eval(start->children[i]));
        break;
        case LUNA:
        functionStack.top().luna.add(eval(start->children[i]));
        break;
        case NOX:
        functionStack.top().nox.add(eval(start->children[i]));
        break;
        case PRINT:
        std::cout<<eval(start->children[i])<<std::endl;
        break;
        case PRINTC:
        std::cout<<char(eval(start->children[i])%128);
        break;
        case OMNIS:
        omnis.push(eval(start->children[i]));
        break;
        }
        break;
        case RIGHTPUSH:
        if(start->children[i]->type==TRASH){

            popper(start);
        }
        else{
            error("Expected TRASH Keyword");
        }
        break;
    }
    }
}

int eval(Node* nt) {
    switch (nt->type) {
        case AND:
            return eval(nt->children[0]) && eval(nt->children[1]);
        case OR:
            return eval(nt->children[0]) || eval(nt->children[1]);
        case EQUALEQUAL:
            return eval(nt->children[0]) == eval(nt->children[1]);

        case BANG:
            return !eval(nt->children[0]);

        case LESSTHAN:
            return eval(nt->children[0]) < eval(nt->children[1]);

        case LESSEQUAL:
            return eval(nt->children[0]) <= eval(nt->children[1]);

        case GREATERTHAN:
            return eval(nt->children[0]) > eval(nt->children[1]);

        case GREATEREQUAL:
            return eval(nt->children[0]) >= eval(nt->children[1]);

        case PLUS:
            return eval(nt->children[0]) + eval(nt->children[1]);

        case MINUS:
            return eval(nt->children[0]) - eval(nt->children[1]);

        case DIVIDE:
            return eval(nt->children[0]) / eval(nt->children[1]);

        case MULTIPLY:
            return eval(nt->children[0]) * eval(nt->children[1]);

        case NUMBER:
            return nt->value.number;  // Replace with the appropriate member if different

        case SOL:
            return functionStack.top().sol.peek();

        case LUNA:
            return functionStack.top().luna.peek();

        case NOX:
            return functionStack.top().nox.peek();
        case INPUT:
            int inp;
            std::cin >> inp;
            return inp;
            case OMNIS:
                if (!omnis.empty()) {
                    return omnis.top();
                } else {
                    error("Omnis stack is empty");
                    return -1;
                }

        default:
            error("Error in evaluation");
            return -1;
    }
}
