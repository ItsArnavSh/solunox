#ifndef CONTAINERS_H
#define CONTAINERS_H
#include "token.h"
class node{
    public:
    int data;
    node* next;
    node* prev;
    node(){}
    node(int data){this->data = data;next = nullptr;prev = nullptr;}
};
class Container{
    TokenType type;
    node* head;
    public:
    Container(){}
    Container(TokenType type){this->type = type;};
    void add(int x);
    void remove(int x);
    void peek();
    void push(int x);
    void pop(int x);
    void enqueue(int x);
    void dequeue(int x);
    void priorityEq(int x);
    void priorityDq(int x);
};
class containers{
    public:
    Container* sol,*luna,*nox;
    containers(TokenType Sol,TokenType Luna,TokenType Nox);
};
#endif
