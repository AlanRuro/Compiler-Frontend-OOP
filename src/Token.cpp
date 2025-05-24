#include "Token.h"

Token::Token(int t) {
    this->tag = t;
}

std::string Token::toString() {
    return std::string(1, static_cast<char>(this->tag));
}
