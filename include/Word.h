#ifndef WORD_H
#define WORD_H
#include "Token.h"

#include <iostream>

class Word : public Token {
    public:
        std::string lexeme = "";
        Word(std::string, int);
        std::string toString() override;
        
        // Python keywords
        static const Word And;
        static const Word Or;
        static const Word Not;
        static const Word True;
        static const Word False;
        static const Word None;
        static const Word If;
        static const Word Elif;
        static const Word Else;
        static const Word For;
        static const Word While;
        static const Word Class;
        static const Word Def;
        static const Word Self;
        static const Word Super;
        static const Word Return;
        static const Word Pass;
        static const Word In;
        static const Word Is;
        static const Word Break;
        static const Word Continue;
        static const Word From;
        static const Word Import;
        
        // static const Word Property;
        // static const Word StaticMethod;
        // static const Word ClassMethod;
        // static const Word AbstractMethod;

};

#endif // WORD_H