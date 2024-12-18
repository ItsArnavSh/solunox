## **Introduction**

**Objective:**  
Our objective is to design a Procedure-Oriented, Purpose-Based Programming Language called `Solunox`, implemented in C++ using a Tree Walk Interpreter. This language serves as an educational tool for mastering Data Structures and Algorithms.

## **Tree Walk Interpreter**

A Tree Walk Interpreter in `Solunox` consists of three core components: **Lexing**, **Parsing**, and **Output Generation**.

- **Lexing**: The lexer converts the source script into a sequence of tokens.
- **Parsing**: The parser accepts the token array, constructs an Abstract Syntax Tree (AST), and performs semantic analysis.
- **Output Generation**: The code is interpreted by traversing the AST, which gives the interpreter its name.

## **Solunox**

`Solunox` is designed with the unique feature of having no variables. This design choice encourages users to become proficient with data structures such as Stacks, Queues, Linked Lists, Trees, and Graphs. The language uses three primary containers whose combinations enable computation. Additionally, an immutable vector is provided for storing and passing data without modifying it.

### **Containers: Sol, Luna, Nox**

These versatile containers can dynamically take on the roles of Stacks, Queues, or Priority Queues.

**Operations:**

- `cont << value`: Push `value` onto `cont`.
- `cont << input("Enter data")`: Insert user input into `cont`.
- `cont`: Retrieve the top element of `cont`.
- `print(cont)`: Print the top element of `cont`.
- `cont = value`: Set the top element of `cont` to `value`.
- `cont1 -> cont2`: Pop from `cont1` and push to `cont2`.
- `cont1 - n -> cont2`: Pop the last `n` elements from `cont1` and push to `cont2`.
- `cont -> n`: Pop and delete the last `n` elements from `cont`.
- `cont?`: Return `false` if `cont` is empty, otherwise `true`.

### **Function Declarations**

Functions are declared with:

```$functionName() { /* body */ }```

### **Control Structures**

- **If-Else**: 
  ```if (condition) { /* code */ } else { /* code */ }```
- **Loop**: Continuous loop until `break`.
- **Switch-Case**: Evaluate an expression, match cases, with a default case.

### **Expressions**

`Solunox` supports the following operations:

`+`, `-`, `*`, `/`, `&&`, `||`, `!`, `==`, `!=`, `<`, `>`.

### **Immutable Array (constas)**

Each function has an immutable array `constas` that stores data but does not allow modifications directly (changes are made via containers).

- `constas << value`: Push `value` onto `constas`.
- `constas?`: Return the length of the array, or `0` if it is empty.
- `constas[3]`: Retrieve the third element from `constas`.

### **Scoping**

- **Global Scope**: `Solunox` does not support a global scope.
- **Function Scope**: Each function has its own scope with a unique set of stacks and arrays.

### **Default Container Types**

Containers start as stacks by default and can be changed using `~spq~`. Containers must be empty before changing types.

- `cont~s;`: Convert `cont` to a Stack.
- `cont~q;`: Convert `cont` to a Queue.
- `cont~p;`: Convert `cont` to a Priority Queue.

### **Non-Primitive Data Structures**

`Solunox` supports Linked Lists, Graphs, and Trees. Their references can be stored in existing data structures and used as needed.

#### **Creating Non-Primitive Data Structures**

- `sol << new (SLL/DLL/CLL/TREE/HEAP/GRAPH)`: Create a new data structure.
- `*sol`: Return the value stored in `sol`.
- `sol ->`: Point to the child node.
- `sol <-`: Point to the parent node.
- `sol ->[0]`: Point to the first node in a tree/graph.
- `a<->b`: Connect nodes (specific to Graphs).
