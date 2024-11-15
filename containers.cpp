#include "containers.h"
#include <iostream>

// Constructor for the containers class, initializing each Container with the appropriate TokenType
containers::containers(TokenType Sol, TokenType Luna, TokenType Nox) {
    this->sol = Container(Sol);
    this->luna = Container(Luna);
    this->nox = Container(Nox);
}

// Constructor for Container class to initialize type and head
Container::Container(TokenType type) : type(type), head(nullptr) {}

// Function to add an element based on the container type
void Container::add(int x) {
    if (type == STACK) {
        std::cout << "Adding to Stack\n";
        push(x);
    } else if (type == QUEUE) {
        std::cout << "Adding to Queue\n";
        enqueue(x);
    } else if (type == PRIORITYQUEUE) {
        std::cout << "Adding to Priority Queue\n";
        priorityEq(x);
    }
}

// Function to remove an element based on the container type
void Container::remove() {
    if (type == STACK) {
        pop();
    } else if (type == QUEUE) {
        dequeue();
    } else if (type == PRIORITYQUEUE) {
        priorityDq();
    }
}

// Peek function to view the top/front element
int Container::peek() {
    if (head) {
        return head->data;
    } else {
        std::cout << "Container is empty.\n";
        return -1;  // or throw an exception if desired
    }
}

// Push function to add an element to the top of the stack
void Container::push(int x) {
    std::cout << "Pushing " << x << " onto Stack\n";
    node* newnode = new node(x);  // Using the node constructor to initialize

    if (!newnode) {
        std::cerr << "Memory allocation failed\n";
        return;
    }

    newnode->next = head;
    newnode->prev = nullptr;

    if (head) {
        head->prev = newnode;
    }

    head = newnode;
}

// Pop function to remove the top element from the stack
void Container::pop() {
    if (!head) {
        std::cout << "Stack is empty. Cannot pop.\n";
        return;
    }
    node* temp = head;
    head = head->next;
    if (head) head->prev = nullptr;
    delete temp;
}

// Enqueue function to add an element to the end of the queue
void Container::enqueue(int x) {
    std::cout << "Enqueueing " << x << " into Queue\n";
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

// Dequeue function to remove the front element from the queue
void Container::dequeue() {
    if (!head) {
        std::cout << "Queue is empty. Cannot dequeue.\n";
        return;
    }
    node* temp = head;
    head = head->next;
    if (head) head->prev = nullptr;
    delete temp;
}

// Function to insert an element into the priority queue based on priority
void Container::priorityEq(int x) {
    std::cout << "Inserting " << x << " into Priority Queue\n";
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

// Function to dequeue (remove) the front element from the priority queue
void Container::priorityDq() {
    if (!head) {
        std::cout << "Priority Queue is empty. Cannot dequeue.\n";
        return;
    }
    node* temp = head;
    head = head->next;
    if (head) head->prev = nullptr;
    delete temp;
}
