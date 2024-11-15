#include "parser.h"
#include "token.h"
#include "errorHandle.h"
#include <unordered_set>
#include <iostream>
std::unordered_set<TokenType> allowedMajors;

void Node::addChild(Node* child) {
    children.push_back(child);
}

// Parser methods
bool Parser::match(TokenType type) {
    return current < tokens.size() && tokens[current].type == type;
}

Token Parser::consume(TokenType type, const std::string& errorMessage) {
    if (match(type)) return tokens[current++];
    error(errorMessage);
}

Token Parser::peek() {
    return tokens[current];
}

void debugger(const std::string& d) {
    std::cout << "Entering parse: " << d << std::endl;
}

Node* Parser::parseProgram() {
    debugger("PROGRAM");
    return parseFunctions();
}

Node* Parser::parseFunctions() {
    debugger("FUNCTIONS");
    Node* functions = new Node(FUNCTIONS);
    while (current < tokens.size()) {
        consume(DOLLAR, "Invalid Function Declaration");
        Token identifier = consume(FUNCTION, "FUNCTION NAME NOT FOUND");
        consume(COPEN, "FUNCTION BLOCK NOT OPENED");
        functions->addChild(parseFunction());
    }
    return functions;
}

Node* Parser::parseFunction() {
    debugger("FUNCTION");
    Node* function = new Node(FUNCTION);
    Token dec = consume(DECLARE, "Container Declaration not found");
    Node* declare = new Node(DECLARE, &dec);
    function->addChild(declare);
    function->addChild(parseStatements());
    return function;
}

Node* Parser::parseStatements() {
    debugger("STATEMENTS");
    Node* statements = new Node(STATEMENTS);
    while (!match(CCLOSE)) {
        if (match(IF)) {
            statements->addChild(parseIf());
        }
        else if (match(LOOP)) {
            statements->addChild(parseLoop());
        }
        else if (match(CALLER)) {
            debugger("Function Call");
            consume(CALLER, "Function Call Operator Expected");
            Token name = consume(FUNCTION, "Function name Expected");
            Node* call = new Node(CALLER, &name);
            statements->addChild(call);
        }
        else if(match(BREAK)){
            Token name = consume(BREAK, "Break Expected");
            Node* call = new Node(BREAK, &name);
             statements->addChild(call);
        }
        else {
            statements->addChild(parseStatement());
        }
    }
    consume(CCLOSE, "Function not ended");
    return statements;
}

Node* Parser::parseIf() {
    debugger("IF");
    consume(IF, "Expected 'if'.");
    Node* ifNode = new Node(IF);

    while (!match(CCLOSE) && !match(DEFAULT)) {
        Node* conditionBranch = new Node(CONDITION_BRANCH);
        conditionBranch->addChild(parseExpression());
        consume(RIGHTPUSH, "Expected '~>' after condition expression.");
        conditionBranch->addChild(parseBlock());
        ifNode->addChild(conditionBranch);
    }

    if (match(DEFAULT)) {
        consume(DEFAULT, "Expected 'default' keyword.");
        consume(RIGHTPUSH, "Expected '~>' after 'default'.");
        Node* defaultBranch = new Node(DEFAULT);
        defaultBranch->addChild(parseBlock());
        ifNode->addChild(defaultBranch);
    }
    consume(CCLOSE, "Expected '}' to close the if statement.");
    return ifNode;
}

Node* Parser::parseLoop() {
    debugger("LOOP");
    consume(LOOP, "Expected 'loop'.");
    Node* loop = new Node(LOOP);
    loop->addChild(parseBlock());
    return loop;
}

Node* Parser::parseBlock() {
    debugger("BLOCK");
    consume(COPEN, "Brackets Expected");
    return parseStatements();
}

Node* Parser::parseStatement() {
    debugger("STATEMENT");
    Token left;
    // Ensure identifier types are handled correctly
    if (match(SOL) || match(LUNA) || match(NOX) || match(CONSTAS) || match(OMNIS) || match(PRINT)) {
        left = consume(peek().type, "Expected identifier.");
        Node* leftAssign = new Node(left.type, &left);

        // Add logic to handle assignment vs other operations
        if (match(EQUAL)|| match(LEFTPUSH)||match(RIGHTPUSH)||match(LEFTCOPY)||match(RIGHTCOPY)) {  // Assuming ASSIGN is the token for '=' or similar
            peek().printToken();
            Node* assignmentNode = new Node(peek().type);
            consume(peek().type, "Expected major after identifier.");
            assignmentNode->addChild(leftAssign);
            assignmentNode->addChild(parseExpression());
            return assignmentNode;
        } else {
            error("Incorrect Statement");
        }
    }

    // For standalone expressions (not assignments), parse normally
    Token curr = peek();
    current++;
    curr.printToken();
    Node* statement = new Node(curr.type);

    // Add parsing for expression directly
    statement->addChild(parseExpression());
    return statement;
}



Node* Parser::parseExpression() {
    debugger("EXPRESSION");
    return parseEquality();
}

Node* Parser::parseEquality() {
    debugger("EQUALITY");
    Node* left = parseComparison();
    while (match(EQUALEQUAL) || match(BANGEQUAL)) {
        Token operatorToken = tokens[current++];
        Node* right = parseComparison();
        Node* equalityNode = new Node(operatorToken.type == EQUALEQUAL ? EQUALEQUAL : BANGEQUAL);
        equalityNode->addChild(left);
        equalityNode->addChild(right);
        left = equalityNode;
    }
    return left;
}

Node* Parser::parseComparison() {
    debugger("COMPARISON");
    Node* left = parseTerm();
    while (match(GREATERTHAN) || match(GREATEREQUAL) || match(LESSTHAN) || match(LESSEQUAL)) {
        Token operatorToken = tokens[current++];
        Node* right = parseTerm();
        Node* comparisonNode;
        switch (operatorToken.type) {
            case GREATERTHAN:       comparisonNode = new Node(GREATERTHAN); break;
            case GREATEREQUAL: comparisonNode = new Node(GREATEREQUAL); break;
            case LESSTHAN:          comparisonNode = new Node(LESSTHAN); break;
            case LESSEQUAL:    comparisonNode = new Node(LESSEQUAL); break;
            default: throw std::runtime_error("Unexpected comparison operator.");
        }
        comparisonNode->addChild(left);
        comparisonNode->addChild(right);
        left = comparisonNode;
    }
    return left;
}

Node* Parser::parseTerm() {
    debugger("TERM");
    Node* left = parseFactor();
    while (match(PLUS) || match(MINUS)) {
        Token operatorToken = tokens[current++];
        Node* right = parseFactor();
        Node* termNode = new Node(operatorToken.type == PLUS ? PLUS : MINUS);
        termNode->addChild(left);
        termNode->addChild(right);
        left = termNode;
    }
    return left;
}

Node* Parser::parseFactor() {
    debugger("FACTOR");
    Node* left = parseUnary();
    while (match(MULTIPLY) || match(DIVIDE)) {
        Token operatorToken = tokens[current++];
        Node* right = parseUnary();
        Node* factorNode = new Node(operatorToken.type == MULTIPLY ? MULTIPLY : DIVIDE);
        factorNode->addChild(left);
        factorNode->addChild(right);
        left = factorNode;
    }
    return left;
}

Node* Parser::parseUnary() {
    debugger("UNARY");
    if (match(BANG) || match(MINUS)) {
        Token operatorToken = tokens[current++];
        Node* right = parseUnary();
        Node* unaryNode = new Node(operatorToken.type == BANG ? BANG : MINUS);
        unaryNode->addChild(right);
        return unaryNode;
    }
    return parsePrimary();
}
Node* Parser::parsePrimary() {
    debugger("Primary");

    if (match(NUMBER)) {
        Token number = consume(NUMBER, "Expected number.");
        return new Node(NUMBER, &number);
    }
    if (match(FUNCTION)) {
        Token identifier = consume(FUNCTION, "Expected function name.");
        return new Node(FUNCTION, &identifier);
    }
    if (match(SOL) || match(LUNA) || match(NOX) || match(CONSTAS) || match(OMNIS) || match(TRASH) || match(INPUT)) {
        return new Node(consume(peek().type, "Expected identifier.").type);
    }
    if (match(BOPEN)) {
        consume(BOPEN, "Expected '('.");
        Node* expr = parseExpression();
        consume(BCLOSE, "Expected ')'.");
        return expr;
    }

    // Improved handling for unexpected tokens, such as `CCLOSE`
    Token unexpected = peek();
    if (unexpected.type == CCLOSE) {
        error("Unexpected '}' encountered, possibly missing a previous '{'.");
    } else {
        unexpected.printToken();
        error("Unexpected token in expression.");
    }
    return nullptr;
}
void Node::printTree(int indent = 0) {
    // Print indentation based on depth level
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }

    // Print the token type of the current node
    std::cout << tokenTypeToString(type) << std::endl;

    // Recursively print each child node, increasing the indentation level
    for (Node* child : children) {
        child->printTree(indent + 1);
    }
}

// Helper function to convert TokenType to string (you'll need this for readable output)
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case PROGRAM: return "PROGRAM";
        case FUNCTIONS: return "FUNCTIONS";
        case FUNCTION: return "FUNCTION";
        case STATEMENTS: return "STATEMENTS";
        case STATEMENT: return "STATEMENT";
        case IF: return "IF";
        case LOOP: return "LOOP";
        case CALLER: return "CALLER";
        case DECLARE: return "DECLARE";
        case EQUALEQUAL: return "EQUALEQUAL";
        case BANG: return "BANG";
        case BANGEQUAL: return "BANGEQUAL";
        case LESSTHAN: return "LESSTHAN";
        case LESSEQUAL: return "LESSEQUAL";
        case GREATERTHAN: return "GREATERTHAN";
        case GREATEREQUAL: return "GREATEREQUAL";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case DIVIDE: return "DIVIDE";
        case MULTIPLY: return "MULTIPLY";
        case EQUAL: return "EQUAL";
        case SOL: return "SOL";
        case LUNA: return "LUNA";
        case NOX: return "NOX";
        case CONSTAS: return "CONSTAS";
        case OMNIS: return "OMNIS";
        case NUMBER: return "NUMBER";
        case LEFTPUSH: return "LEFTPUSH";
        case LEFTCOPY: return "LEFTCOPY";
        case RIGHTPUSH: return "RIGHTPUSH";
        case RIGHTCOPY: return "RIGHTCOPY";
        default: return "UNKNOWN";
    }
}
