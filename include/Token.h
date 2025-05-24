#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

enum class Tag {
    // Keywords
    CLASS,
    DEF,
    SELF,
    SPECIAL_NAME,
    SUPER,  // Added super
    NONE,   // Added None
    TRUE,
    FALSE,
    RETURN,
    PASS,
    IN,
    IS,
    IF,
    ELIF,
    ELSE,
    FOR,
    WHILE,
    BREAK,
    CONTINUE,
    
    // // OOP specific
    DECORATOR,      // Added @decorator
    // PROPERTY,       // Added @property
    // STATICMETHOD,   // Added @staticmethod
    // CLASSMETHOD,    // Added @classmethod
    // ABSTRACTMETHOD, // Added @abstractmethod
    
    // Punctuation
    COLON,
    COMMA,
    DOT,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    OPEN_BRACKET,   // Added for list access [index]
    CLOSE_BRACKET,  // Added for list access [index]
    OPEN_BRACE,
    CLOSE_BRACE,
    ASSIGN,
    ARROW,
    
    // Indentation
    INDENT,
    DEDENT,
    NEWLINE,
    
    // Types
    TYPE,
    VARIABLE,
    NUM,
    STRING,
    
    // Operators
    LOGIC_OP,
    PLUS,
    MINUS,
    MULT,
    DIV,
    MOD,        // Added %
    AND,
    OR,
    NOT,

    FROM,
    IMPORT,

    POO_DECORATOR
};

class Token {
    public:
        int tag;
        Token(int t);
        virtual std::string toString();
};

#endif // TOKEN_H