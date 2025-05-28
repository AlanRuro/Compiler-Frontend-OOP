#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "TokenStream.h"
#include <iostream>

class Parser {
    public:
        // Constructor que acepta TokenStream
        Parser(TokenStream* stream);
        virtual ~Parser() = default;
        virtual void parse() = 0;

    protected:
        TokenStream* stream;
        Token* look;

        void move();
        void match(int tag);
        void error(const std::string& message);
        void debug(const std::string& message);
        bool isType(int tag);
};

#endif // PARSER_H