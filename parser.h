#ifndef  PARSER_H
#define PARSER_H
#include "token.h"
#include "error.h"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>
class Node {
public:
    TokenType type;
    Storage value;
    std::vector<Node*> children;

    // Constructor for non-terminal nodes
    Node(TokenType type) : type(type) {}

    // Constructor for terminal nodes (tokens)
    Node(TokenType type, Token* token) : type(type), value(token->value) {}

    void addChild(Node* child);
    void printTree(int indent);
};

class Parser{
    public:
    std::vector<Token> tokens;
    uint current = 0;
    Parser(std::vector<Token> tokens): tokens(tokens){};
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& errorMessage);
    Token peek();
    Node* parseProgram();
    Node* parseFunctions();
    Node *parseFunction();
    Node* parseStatements();
    Node* parseStatement();
    Node* parseBlock();
    Node* parseOperator();
    Node* parseLoop();
    Node* parseIf();
    Node* parseExpression();
    Node* parseEquality();
    Node* parseComparison();
    Node* parseTerm();
    Node* parseFactor();
    Node* parseUnary();
    Node* parsePrimary();
};
std::string tokenTypeToString(TokenType);
#endif
