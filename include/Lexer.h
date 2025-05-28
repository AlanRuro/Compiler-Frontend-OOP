#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <stack>
#include <queue>
#include "Token.h"
#include "Word.h"
#include "Num.h"
#include "TokenStream.h"

class Lexer {
    private:
        char peek = '\0';
        std::ifstream source;
        int line = 1;
        int column = 0;
        bool line_start = true;  // Added to track line start
        int spaces = 0; // Added to track spaces and identation
        std::stack<int> indent_stack;  // Stack to track indentation levels
        std::queue<Token*> dedent_queue;  // Queue for pending dedent tokens
        std::unordered_map<std::string, Word*> words;
        
        void reserve(Word w);
        void readch();
        bool readch(char c);

        void skipWhitespace(bool at_line_start);
        Token* handlePendingDedents();
        Token* handleDedents();
        Token* handleIdent();
        Token* handleNewLines();
        void handleComments();
        Token* handleEOF();
        Token* handleVariables(int tag);
        Token* handleNumbers();
        Token* handleStrings();
        Token* handleOperators();
        Token* handlePunctuation();
        Word* findKeyword(std::string word);
        Token* scan();
        
    public:
        Lexer(std::string filename);
        ~Lexer();
        
        TokenStream* generateStream();
        
        int get_line() const { return line; }
        int get_column() const { return column; }
};

#endif // LEXER_H