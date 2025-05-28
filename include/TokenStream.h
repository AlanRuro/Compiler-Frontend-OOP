#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

#include <vector>
#include <memory>
#include "Token.h"

class TokenStream {
private:
    std::vector<std::unique_ptr<Token>> tokens;
    size_t current_pos = 0;

public:
    TokenStream() = default;
    ~TokenStream() = default;

    void addToken(Token* token);
    Token* peek() const;
    Token* next();
    void reset();
    size_t size() const;
    Token* at(size_t pos) const;
    size_t position() const;
    void setPosition(size_t pos);
};

#endif // TOKENSTREAM_H 