#include "scanner.h"
#include "token.h"
#include <iostream>
#include <vector>
#include <cctype>
std::unordered_map<std::string,TokenType> keywords;
void loadKeywords(){
    keywords["input"] = INPUT;
    keywords["print"] = PRINT;
    keywords["sol"] = SOL;
    keywords["luna"] = LUNA;
    keywords["nox"] = NOX;
    keywords["constas"] = CONSTAS;
    keywords["omnis"] = OMNIS;
    keywords["if"] = IF;
    keywords["else"] = ELSE;
    keywords["loop"] = LOOP;
    keywords["trash"] = TRASH;
    keywords["break"] = BREAK;
}
std::vector<Token> Scanner(std::string filename) {
    loadKeywords();
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
            case '<': {
                char next = reader.readNextChar();
                if (next == '~') {
                    tokens.push_back(Token(LEFTPUSH));
                } else if (next == '<') {
                    tokens.push_back(Token(LEFTCOPY));
                } else if (next == '=') {
                    tokens.push_back(Token(LESSEQUAL));
                } else {
                    reader.goBack(); // Move back if it's just '<'
                    tokens.push_back(Token(LESSTHAN));
                }
                break;
            }

            case '>': {
                char next = reader.readNextChar();
                if (next == '>') {
                    tokens.push_back(Token(RIGHTCOPY));
                } else if (next == '=') {
                    tokens.push_back(Token(GREATEREQUAL));

                } else {
                    reader.goBack(); // Move back if it's just '>'
                    tokens.push_back(Token(GREATERTHAN));
                }
                break;
            }
                case ':':{
                    std::string pattern = "";
                    pattern+=reader.readNextChar();
                    pattern+=reader.readNextChar();
                    pattern+=reader.readNextChar();
                    short k=0;
                    if(reader.readNextChar()==':'){
                        for(auto cont:pattern){
                            switch(cont){
                                case 's':{
                                    k = k*3+0;
                                    break;
                                }
                                case 'q':{
                                    k = k*3+1;
                                    break;
                                }
                                case 'p':{
                                    k=k*3+2;
                                    break;
                                }
                                default:{
                                    error("Invalid Container Initialization");
                                    break;
                                }

                            }
                        }
                        tokens.push_back(Token(DECLARE,k));
                    }
                    else{
                        error("Invalid Container Initialization");
                    }
                    break;
                }
                case '~': {
                    if(reader.readNextChar()=='>'){
                        tokens.push_back(RIGHTPUSH);
                    }
                    break;
                }


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
                    if (keywords.find(identifier)!=keywords.end()) {
                        tokens.push_back(Token(keywords[identifier]));

                    } else {
                        if(identifier=="main"){
                            mainLocation = counter;
                        }
                        tokens.push_back(Token(identifier)); // Treat as a general identifier
                    }
                }
                break;
        }
    }

    return tokens;
}
