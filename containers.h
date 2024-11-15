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
    Container() : head(nullptr) {}
    Container(TokenType type);
    void add(int x);
    void remove();
    int peek();
    void push(int x);
    void pop();
    void enqueue(int x);
    void dequeue();
    void priorityEq(int x);
    void priorityDq();
};
class containers{
    public:
    Container sol,luna,nox;
    containers(TokenType Sol,TokenType Luna,TokenType Nox);
};
#endif
