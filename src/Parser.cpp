#include "Parser.h"
#include "Lexer.h"

Parser::Parser(TokenStream* stream) : stream(stream) {
    move(); // Get the first token
}

void Parser::move() {
    look = stream->next();
}

void Parser::error(const std::string& message) {
    std::string errorMsg = "Syntax error at token position " + std::to_string(stream->position()) +
                          ": " + message;
    throw std::runtime_error(errorMsg);
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