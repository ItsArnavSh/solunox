#include "walker.h"
#include "containers.h"
#include "token.h"
#include <iostream>
#include <stack>

void deer(){}
std::stack<containers> functionStack;
std::stack<int> omnis;
Node* root;
void mainWalker(Node *nt){
    root = nt;
    if(mainLocation==-1){
        error("No Main Function found");
    }
    solveFunction(nt->children[mainLocation]);
}
void solveFunction(Node* start){

    containers func(STACK,STACK,STACK);
    functionStack.push(func);
    //start[1] is declare, do it later
    //start[2] is statements
    solveStatements(start->children[1]);
    functionStack.pop();//Remove Function from stack after operation
}
void solveStatements(Node* start){
    for(auto *statement : start->children){
        solveAnyStatement(statement);

    }
}
void solveAnyStatement(Node* statement){
    if(statement->type==CALLER){
        solveFunction(root->children[statement->value.identifierKey]);
    }
    else if(statement->type==LOOP){
        solveLoop(statement->children[0]);
    }
    else
    solveStatement(statement);
}
void solveIF(Node* start){}
void solveLoop(Node* start){
    while(true){
        for(auto *statement : start->children){
            if(statement->type==BREAK){
                goto out;
            }
            else{
                solveAnyStatement(statement);
            }
        }
    }
    out:

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
    switch(start->type){
        case LEFTPUSH:
        switch(start->children[0]->type){
        case SOL:
        functionStack.top().sol.add(eval(start->children[1]));
        popper(start);
        break;
        case LUNA:
        functionStack.top().luna.add(eval(start->children[1]));
        popper(start);
        break;
        case NOX:
        functionStack.top().nox.add(eval(start->children[1]));
        popper(start);
        break;
        case PRINT:
        std::cout<<eval(start->children[1])<<std::endl;
        popper(start);
        break;
        case OMNIS:
        omnis.push(eval(start->children[1]));
        popper(start);
        break;
        default:
        error("Container or Print Expected");
        }
        break;
        case LEFTCOPY:
        switch(start->children[0]->type){
        case SOL:
        functionStack.top().sol.add(eval(start->children[1]));
        break;
        case LUNA:
        functionStack.top().luna.add(eval(start->children[1]));
        break;
        case NOX:
        functionStack.top().nox.add(eval(start->children[1]));
        break;
        case PRINT:
        std::cout<<eval(start->children[1])<<std::endl;
        popper(start);
        break;
        case OMNIS:
        omnis.push(eval(start->children[1]));
        break;
        }
        break;
        case RIGHTPUSH:
        if(start->children[1]->type==TRASH){
            std::cout << "Deleting";
            popper(start);
        }
        else{
            error("Expected TRASH Keyword");
        }
        break;
    }
}

int eval(Node* nt) {
    switch (nt->type) {
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
