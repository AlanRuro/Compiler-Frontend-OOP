#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include <iostream>

class Parser {
    public:
        Parser(Lexer* lexer);
        virtual void parse() = 0;

    protected:
        Lexer* lexer;
        Token* look;

        void move();
        void match(int tag);
        void error(const std::string& message);
        void debug(const std::string& message);
        bool isType(int tag);

};

#endif // PARSER_H