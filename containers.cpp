
#include "containers.h"
#include <iostream>

void Container::add(int x) {
    if (type == STACK) {
        push(x);
    } else if (type == QUEUE) {
        enqueue(x);
    } else if (type == PRIORITYQUEUE) {
        priorityEq(x);
    }
}

void Container::remove(int x) {
    if (type == STACK) {
        pop(x);
    } else if (type == QUEUE) {
        dequeue(x);
    } else if (type == PRIORITYQUEUE) {
        priorityDq(x);
    }
}

void Container::peek() {
    if (head) {
        std::cout << "Top element: " << head->data << std::endl;
    } else {
        std::cout << "Container is empty.\n";
    }
}

void Container::push(int x) {
    node* newnode = new node(x);
    newnode->next = head;
    if (head) head->prev = newnode;
    head = newnode;
}

void Container::pop(int x) {
    if (!head) return;
    node* temp = head;
    head = head->next;
    if (head) head->prev = nullptr;
    delete temp;
}

void Container::enqueue(int x) {
    node* newnode = new node(x);
    if (!head) {
        head = newnode;
    } else {
        node* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newnode;
        newnode->prev = temp;
    }
}

void Container::dequeue(int x) {
    if (!head) return;
    node* temp = head;
    head = head->next;
    if (head) head->prev = nullptr;
    delete temp;
}

void Container::priorityEq(int x) {
    node* newnode = new node(x);
    if (!head || head->data > x) {
        newnode->next = head;
        if (head) head->prev = newnode;
        head = newnode;
    } else {
        node* temp = head;
        while (temp->next && temp->next->data <= x) temp = temp->next;
        newnode->next = temp->next;
        if (temp->next) temp->next->prev = newnode;
        temp->next = newnode;
        newnode->prev = temp;
    }
}

void Container::priorityDq(int x) {
    if (!head) return;
    node* temp = head;
    head = head->next;
    if (head) head->prev = nullptr;
    delete temp;
}

containers::containers(TokenType Sol,TokenType Luna,TokenType Nox){
    this->sol = new Container(Sol);
    this->luna = new Container(Luna);
    this->nox = new Container(Nox);
}
