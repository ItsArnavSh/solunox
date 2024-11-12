#ifndef WALKER_H
#define WALKER_H
#include "parser.h"
#include "errorHandle.h"
#include "token.h"
#include "scope.h"
#include <iostream>
void mainWalker(Node* nt);
void solveFunction(Node* start);
void solveStatements(Node* start);
void solveIF(Node* start);
void solveLoop(Node* start);
void solveStatement(Node* start);
int eval(Node* nt);
void printAll();
#endif
