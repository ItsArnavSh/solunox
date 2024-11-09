#include "scanner.h"
#include "token.h"
#include <iostream>
#include <vector>
#include <cctype>

std::vector<Token> Scanner(std::string filename) {
    FileReader reader(filename);
    std::vector<Token> tokens;

    while (!reader.isEOF()) {
        char current = reader.readNextChar();

        switch (current) {
            case '+':
                tokens.push_back(Token(PLUS));
                break;

            case '-':
                tokens.push_back(Token(MINUS));
                break;

            case '*':
                tokens.push_back(Token(MULTIPLY));
                break;

            case '/': {
                char next = reader.readNextChar();
                if (next == '/') {
                    // Skip the rest of the line for comments
                    while (next != '\n' && !reader.isEOF()) {
                        next = reader.readNextChar();
                    }
                } else {
                    reader.goBack(); // Move back if it's just '/'
                    tokens.push_back(Token(DIVIDE));
                }
                break;
            }

            case '=': {
                char next = reader.readNextChar();
                if (next == '=') {
                    tokens.push_back(Token(EQUALEQUAL));
                } else {
                    reader.goBack(); // Move back if it's just '='
                    tokens.push_back(Token(EQUAL));
                }
                break;
            }

            case '!': {
                char next = reader.readNextChar();
                if (next == '=') {
                    std::cout<<"NOT";
                    tokens.push_back(Token(BANGEQUAL));
                } else {
                    std::cout<<"Here";
                    reader.goBack(); // Move back if it's just '!'
                    tokens.push_back(Token(BANG));
                }
                break;
            }

            case '<': {
                char next = reader.readNextChar();
                if (next == '=') {
                    tokens.push_back(Token(LESSEQUAL));
                } else {
                    reader.goBack(); // Move back if it's just '<'
                    tokens.push_back(Token(LESSTHAN));
                }
                break;
            }

            case '>': {
                char next = reader.readNextChar();
                if (next == '=') {
                    tokens.push_back(Token(GREATEREQUAL));
                } else {
                    reader.goBack(); // Move back if it's just '>'
                    tokens.push_back(Token(GREATERTHAN));
                }
                break;
            }

            case '(':
                tokens.push_back(Token(BOPEN));
                break;

            case ')':
                tokens.push_back(Token(BCLOSE));
                break;

            case '{':
                tokens.push_back(Token(COPEN));
                break;

            case '}':
                tokens.push_back(Token(CCLOSE));
                break;

            case ';':
                tokens.push_back(Token(SEMICOLON));
                break;

            case '$':
                tokens.push_back(Token(DOLLAR));
                break;

            case '@':
                tokens.push_back(Token(CALLER));
                break;

            default:
                // Handle numbers
                if (std::isdigit(current)) {
                    int number = current - '0';
                    char next = reader.readNextChar();
                    while (!reader.isEOF() && std::isdigit(next)) {
                        number = number * 10 + (next - '0');
                        next = reader.readNextChar();
                    }
                    reader.goBack(); // Step back one character
                    tokens.push_back(Token(number));
                }
                // Handle identifiers and keywords
                else if (std::isalpha(current) || current == '_') {
                    std::string identifier;
                    identifier += current;
                    char next = reader.readNextChar();
                    while (!reader.isEOF() && (std::isalnum(next) || next == '_')) {
                        identifier += next;
                        next = reader.readNextChar();
                    }
                    reader.goBack(); // Step back one character
                    if (identifier == "function") {
                        tokens.push_back(Token(FUNCTION));
                    } else {
                        tokens.push_back(Token(identifier)); // Treat as a general identifier
                    }
                }
                break;
        }
    }

    return tokens;
}
