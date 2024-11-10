#ifndef TOKEN_H
#define TOKEN_H

#include <cstdint>
#include <string>
#include <unordered_map>


// Union to store different token values
typedef union Storage {
    int number;
    void* empty;
    short identifierKey;

} Storage;

// Global counter for new identifier keys
extern short counter;

// Map to store identifier strings
extern std::unordered_map<short, std::string> functions;

// Enumeration for token types
typedef enum TokenType {
    PLUS,//+
    MINUS,//-
    DIVIDE,///
    MULTIPLY,//*
    EQUAL,//=
    FUNCTION,//functionname
    SEMICOLON,//;
    BOPEN,//(
    BCLOSE,//)
    NUMBER,
    BANG,
    EQUALEQUAL,//==
    BANGEQUAL,//!=
    LESSTHAN,//<
    LESSEQUAL,//<=
    GREATERTHAN,//>
    GREATEREQUAL,//>=
    COPEN,//{
    CCLOSE,//}
    DOLLAR,//$
    CALLER,//@
    //KEYWORDS
    IF,
    ELSE,
    LOOP,
    INPUT,
    PRINT,
    SOL,
    LUNA,
    NOX,
    CONSTAS,
    OMNIS,

    //SPECIAL
    DECLARE,//~abc~ //abc can be 123, all 9 values are saved as numbers
    LEFTPUSH,//<- <~4
    RIGHTPUSH,//-> ~4~>
    LEFTCOPY,//<< <<~4~
    RIGHTCOPY,//>>

} TokenType;

class Token {
public:
    TokenType type;
    Storage value;

    // Constructors for different token types
    Token(TokenType type);           // For operators
    Token(std::string literal);      // For identifiers (sets identifierKey)
    Token(int number);               // For numbers (sets number)
    Token (TokenType type,int number);//For push and copy values
    Token (TokenType type,std::string str);
    void printToken();
};

#endif
