#include "token.h"
#include <algorithm>
#include "error.h"

short counter = 0;  // Initialize counter
std::unordered_map<short, std::string> functions;

Token::Token(TokenType type) {
    this->type = type;
    this->value.empty = nullptr;
}

Token::Token(std::string literal) {
    this->type = FUNCTION;

    // Check if literal already exists in the map to avoid duplicate entries
    auto it = std::find_if(functions.begin(), functions.end(),[&literal](const auto& pair) { return pair.second == literal; });

    if (it != functions.end()) {
        this->value.identifierKey = it->first; // Use existing key
    } else {
        functions[counter] = literal;         // Add new functions
        this->value.identifierKey = counter;
        counter++;
    }
}
Token::Token (TokenType type,int number){
    this->type = type;
    this->value.number = number;
}
Token::Token(int number) {
    this->type = NUMBER;
    this->value.number = number;
}
Token::Token (TokenType type,std::string str){
    this->type = DECLARE;

}
#include <iostream>
#include "token.h"

void Token::printToken() {
    // Print token type
    switch (type) {
        case PLUS:
            std::cout << "Token Type: PLUS";
            break;
        case MINUS:
            std::cout << "Token Type: MINUS";
            break;
        case DIVIDE:
            std::cout << "Token Type: DIVIDE";
            break;
        case MULTIPLY:
            std::cout << "Token Type: MULTIPLY";
            break;
        case EQUAL:
            std::cout << "Token Type: EQUAL";
            break;
        case FUNCTION:
            std::cout << "Token Type: FUNCTION";
            std::cout << ", Value: " << value.identifierKey;
            break;
        case SEMICOLON:
            std::cout << "Token Type: SEMICOLON";
            break;
        case BOPEN:
            std::cout << "Token Type: BOPEN (";
            break;
        case BCLOSE:
            std::cout << "Token Type: BCLOSE )";
            break;
        case NUMBER:
            std::cout << "Token Type: NUMBER, Value: " << value.number;
            break;
        case BANG:
            std::cout << "Token Type: BANG (!)";
            break;
        case EQUALEQUAL:
            std::cout << "Token Type: EQUALEQUAL (==)";
            break;
        case BANGEQUAL:
            std::cout << "Token Type: BANGEQUAL (!=)";
            break;
        case LESSTHAN:
            std::cout << "Token Type: LESSTHAN (<)";
            break;
        case LESSEQUAL:
            std::cout << "Token Type: LESSEQUAL (<=)";
            break;
        case GREATERTHAN:
            std::cout << "Token Type: GREATERTHAN (>)";
            break;
        case GREATEREQUAL:
            std::cout << "Token Type: GREATEREQUAL (>=)";
            break;
        case COPEN:
            std::cout << "Token Type: COPEN ({)";
            break;
        case CCLOSE:
            std::cout << "Token Type: CCLOSE (})";
            break;
        case DOLLAR:
            std::cout << "Token Type: DOLLAR ($)";
            break;
        case CALLER:
            std::cout << "Token Type: CALLER (@)";
            break;

        // Keywords
        case IF:
            std::cout << "Token Type: IF";
            break;
        case ELSE:
            std::cout << "Token Type: ELSE";
            break;
        case LOOP:
            std::cout << "Token Type: LOOP";
            break;
        case INPUT:
            std::cout << "Token Type: INPUT";
            break;
        case PRINT:
            std::cout << "Token Type: PRINT";
            break;
        case SOL:
            std::cout << "Token Type: SOL";
            break;
        case LUNA:
            std::cout << "Token Type: LUNA";
            break;
        case NOX:
            std::cout << "Token Type: NOX";
            break;
        case CONSTAS:
            std::cout << "Token Type: CONSTAS";
            break;
        case OMNIS:
            std::cout << "Token Type: OMNIS";
            break;

        // Special tokens
        case DECLARE:
            std::cout << "Token Type: DECLARE (~abc~)";
            break;
        case LEFTPUSH:
            std::cout << "Token Type: LEFTPUSH (<-)";
            break;
        case RIGHTPUSH:
            std::cout << "Token Type: RIGHTPUSH (->)";
            break;
        case LEFTCOPY:
            std::cout << "Token Type: LEFTCOPY (<<)";
            break;
        case RIGHTCOPY:
            std::cout << "Token Type: RIGHTCOPY (>>)";
            break;

        default:
            std::cout << "Unknown Token Type";
    }
    std::cout << std::endl;
}
