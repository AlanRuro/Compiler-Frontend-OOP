#include "TokenStream.h"

void TokenStream::addToken(Token* token) {
    tokens.push_back(std::unique_ptr<Token>(token));
}

Token* TokenStream::peek() const {
    if (current_pos >= tokens.size()) {
        return nullptr;
    }
    return tokens[current_pos].get();
}

Token* TokenStream::next() {
    if (current_pos >= tokens.size()) {
        return nullptr;
    }
    return tokens[current_pos++].get();
}

void TokenStream::reset() {
    current_pos = 0;
}

size_t TokenStream::size() const {
    return tokens.size();
}

Token* TokenStream::at(size_t pos) const {
    if (pos >= tokens.size()) {
        return nullptr;
    }
    return tokens[pos].get();
}

size_t TokenStream::position() const {
    return current_pos;
}

void TokenStream::setPosition(size_t pos) {
    if (pos <= tokens.size()) {
        current_pos = pos;
    }
}