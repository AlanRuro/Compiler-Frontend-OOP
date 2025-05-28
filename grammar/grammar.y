%{
#include <stdio.h>
#include "Token.h"
#include "Lexer.h"

// Variables para tracking OOP
bool hasClass = false;
bool hasInheritance = false;
bool hasInit = false;
bool hasSelf = false;
bool hasSuper = false;
bool hasDecorator = false;
%}

// Tokens
%token CLASS DEF SELF CLS TYPE VARIABLE INIT MULT
%token PROPERTY STATICMETHOD CLASSMETHOD ABSTRACTMETHOD
%token COLON COMMA OPEN_PARENTHESIS CLOSE_PARENTHESIS
%token ASSIGN ARROW NEWLINE INDENT DEDENT

%%

// Program Structure
program
    : preSkipStatements elements postSkipStatements
    ;

elements
    : element moreElements
    ;

moreElements
    : element moreElements
    | %empty
    ;

element
    : classDefs
    | methodDefs
    ;

// Class Definitions
classDefs
    : classDef moreClassDefs
    ;

moreClassDefs
    : classDef moreClassDefs
    | %empty
    ;

classDef
    : CLASS VARIABLE inheritance COLON classSuite
    ;

inheritance
    : OPEN_PARENTHESIS parentList CLOSE_PARENTHESIS
    | %empty
    ;

parentList
    : VARIABLE moreParents
    ;

moreParents
    : COMMA VARIABLE moreParents
    | %empty
    ;

// Method Definitions
methodDefs
    : methodDef moreMethodDefs
    ;

moreMethodDefs
    : methodDef moreMethodDefs
    | %empty
    ;

methodDef
    : CLASSMETHOD NEWLINE methodDefCls
    | PROPERTY NEWLINE methodDefSelf
    | STATICMETHOD NEWLINE methodDefRaw
    | ABSTRACTMETHOD NEWLINE methodDefSelf
    | methodDefSelf
    ;

methodDefRaw
    : DEF methodName OPEN_PARENTHESIS paramList CLOSE_PARENTHESIS methodDefTail
    ;

methodDefSelf
    : DEF methodName OPEN_PARENTHESIS SELF moreParams CLOSE_PARENTHESIS methodDefTail
    ;

methodDefCls
    : DEF methodName OPEN_PARENTHESIS CLS moreParams CLOSE_PARENTHESIS methodDefTail
    ;

methodDefTail
    : returnType COLON methodSuite
    ;

methodName
    : INIT
    | VARIABLE
    ;

paramList
    : parameter moreParams
    | %empty
    ;

moreParams
    : COMMA parameter moreParams
    | %empty
    ;

parameter
    : paramName typeHint defaultValue
    ;

paramName
    : VARIABLE
    | MULT VARIABLE
    | MULT MULT VARIABLE
    ;

typeHint
    : COLON TYPE
    | %empty
    ;

defaultValue
    : ASSIGN skipDefault
    | %empty
    ;

returnType
    : ARROW TYPE
    | %empty
    ;

// Suite and Statements
classSuite
    : NEWLINE INDENT classBody DEDENT
    ;

methodSuite
    : NEWLINE INDENT methodBody DEDENT
    ;

methodBody
    : skipStatement methodBody
    | %empty
    ;

classBody
    : methodDefs methodBody
    | %empty
    ;

// Skip statements (no parsing, just token consumption)
preSkipStatements
    : %empty
    ;

skipStatement
    : %empty
    ;

skipDefault
    : %empty
    ;

postSkipStatements
    : %empty
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}