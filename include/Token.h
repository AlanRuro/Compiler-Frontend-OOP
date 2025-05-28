#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

enum class Tag {
    // Keywords
    CLASS,
    DEF,
    SELF,
    INIT,
    SUPER,
    NONE,   
    ANY,
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
    DECORATOR,
    PROPERTY,       // @property
    STATICMETHOD,   // @staticmethod
    CLASSMETHOD,    // @classmethod
    ABSTRACTMETHOD, // @abstractmethod
    CLS,            // cls for @classmethod
    
    // Punctuation
    COLON,
    COMMA,
    DOT,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    OPEN_BRACKET,
    CLOSE_BRACKET,  
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
    DOCSTRING,
    
    // Operators
    LOGIC_OP,
    PLUS,
    MINUS,
    MULT,
    DIV,
    MOD,
    AND,
    OR,
    NOT,

    FROM,
    IMPORT
};

class Token {
    public:
        int tag;
        Token(int t);
        virtual ~Token() = default;
        virtual std::string toString();
};

#endif // TOKEN_H