// Parser.cpp
#include "Parser.h"
#include <stdexcept>
#include <iostream>

Parser::Parser(Lexer* l) : lexer(l) {
    move(); // Get the first token
}

void Parser::move() {
    look = lexer->scan();
}

void Parser::error(const std::string& message) {
    std::string errorMsg = "Syntax error at line " + std::to_string(lexer->get_line()) +
                          ": " + message;
    throw 1;
    // throw std::runtime_error(errorMsg);
}

void Parser::match(int tag) {
    if (look && look->tag == tag) {
        move();
    } else {
        error("Expected token with tag " + std::to_string(tag));
    }
}

bool Parser::isType(int tag) {
    return look && look->tag == tag;
}

void Parser::debug(const std::string& message) {
    std::cout << "Non-terminal in use: " << message << std::endl;
}