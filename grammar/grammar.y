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

// Tokens específicos para POO
%token CLASS DEF SELF SUPER
%token DECORATOR PROPERTY STATICMETHOD CLASSMETHOD ABSTRACTMETHOD
%token COLON COMMA DOT OPEN_PARENTHESIS CLOSE_PARENTHESIS
%token OPEN_BRACKET CLOSE_BRACKET OPEN_BRACE CLOSE_BRACE
%token ASSIGN ARROW NEWLINE
%token TYPE VARIABLE SPECIAL_NAME
%token STRING NUM
%token IF TRY EXCEPT PASS WHILE
%token EQUALS PLUS MINUS MULT DIV
%token RETURN IN
%token LT GT LE GE NE
%token AND OR NOT
%token AT

%%

// Punto de entrada - solo acepta un statement a la vez
program
    : oop_statement
    ;

// Un solo statement POO
oop_statement
    : method_definition          // def method(self, args): return ...
    | decorator_statement       // @property
    | self_attribute_access     // self.attr.method()
    | self_attribute_assign     // self.attr = value
    | self_method_call         // self.method()
    | if_self_statement        // if self.attr == value: ...
    | while_self_statement     // while self.attr == value: ...
    | try_self_statement       // try: self.method() except: ...
    | return_statement         // return type(self)(...)
    | super_call              // super().method()
    ;

// Decorador
decorator_statement
    : AT VARIABLE
    {
        hasDecorator = true;
    }
    ;

// Definición de método con return
method_definition
    : DEF method_name OPEN_PARENTHESIS SELF param_list_prime CLOSE_PARENTHESIS return_type COLON return_statement
    {
        hasSelf = true;
        if ($2 == "__init__") hasInit = true;
    }
    ;

// Acceso a atributos de self con llamadas a métodos
self_attribute_access
    : SELF DOT attribute_chain_prime
    {
        hasSelf = true;
    }
    ;

attribute_chain_prime
    : VARIABLE method_call_chain_prime
    | SPECIAL_NAME method_call_chain_prime
    | VARIABLE OPEN_BRACKET expression CLOSE_BRACKET method_call_chain_prime
    ;

method_call_chain_prime
    : DOT method_call method_call_chain_prime
    | %empty
    ;

// Asignación a atributos de self
self_attribute_assign
    : SELF DOT attribute_chain_prime ASSIGN expression
    {
        hasSelf = true;
    }
    ;

// Llamada a método de self
self_method_call
    : SELF DOT method_name OPEN_PARENTHESIS arg_list_prime CLOSE_PARENTHESIS
    {
        hasSelf = true;
    }
    ;

// Llamada a super
super_call
    : SUPER OPEN_PARENTHESIS CLOSE_PARENTHESIS DOT method_name OPEN_PARENTHESIS arg_list_prime CLOSE_PARENTHESIS
    {
        hasSuper = true;
    }
    ;

// If statement con self
if_self_statement
    : IF self_condition COLON statement
    {
        hasSelf = true;
    }
    ;

// While statement con self
while_self_statement
    : WHILE self_condition COLON statement
    {
        hasSelf = true;
    }
    ;

self_condition
    : SELF DOT VARIABLE comparison_op expression
    | SELF DOT VARIABLE IN expression
    | self_condition AND self_condition
    | self_condition OR self_condition
    | NOT self_condition
    ;

comparison_op
    : EQUALS
    | NE
    | LT
    | GT
    | LE
    | GE
    ;

// Try statement con self
try_self_statement
    : TRY COLON self_statement_list_prime except_clauses
    {
        hasSelf = true;
    }
    ;

self_statement_list_prime
    : self_statement self_statement_list_prime
    | %empty
    ;

self_statement
    : self_method_call
    | self_attribute_access
    | self_attribute_assign
    | super_call
    | PASS
    ;

except_clauses
    : except_clause except_clauses
    | %empty
    ;

except_clause
    : EXCEPT VARIABLE COLON statement
    | EXCEPT OPEN_PARENTHESIS VARIABLE CLOSE_PARENTHESIS COLON statement
    ;

// Return statement
return_statement
    : RETURN expression
    ;

// Nombre de método (normal o especial)
method_name
    : SPECIAL_NAME
    {
        if ($1 == "__init__") hasInit = true;
    }
    | VARIABLE
    ;

// Lista de parámetros (sin recursión izquierda)
param_list_prime
    : COMMA VARIABLE type_hint default_value param_list_prime
    | %empty
    ;

// Tipo de parámetro
type_hint
    : COLON TYPE
    | %empty
    ;

// Valor por defecto
default_value
    : ASSIGN expression
    | %empty
    ;

// Tipo de retorno
return_type
    : ARROW TYPE
    | %empty
    ;

// Lista de argumentos (sin recursión izquierda)
arg_list_prime
    : expression arg_list_tail
    | named_arg arg_list_tail
    | %empty
    ;

arg_list_tail
    : COMMA expression arg_list_tail
    | COMMA named_arg arg_list_tail
    | %empty
    ;

named_arg
    : VARIABLE ASSIGN expression
    ;

// Expresiones básicas
expression
    : VARIABLE
    | STRING
    | NUM
    | method_call
    | attribute_access
    | binary_expression
    | comparison_expression
    | logical_expression
    | list_literal
    | tuple_literal
    | type_expression
    | ternary_expression
    ;

// Llamada a método
method_call
    : VARIABLE OPEN_PARENTHESIS arg_list_prime CLOSE_PARENTHESIS
    | attribute_access OPEN_PARENTHESIS arg_list_prime CLOSE_PARENTHESIS
    ;

// Acceso a atributos
attribute_access
    : VARIABLE DOT attribute_chain_prime
    | VARIABLE OPEN_BRACKET expression CLOSE_BRACKET
    ;

// Expresiones binarias
binary_expression
    : expression PLUS expression
    | expression MINUS expression
    | expression MULT expression
    | expression DIV expression
    ;

// Expresiones de comparación
comparison_expression
    : expression comparison_op expression
    | expression IN expression
    ;

// Expresiones lógicas
logical_expression
    : expression AND expression
    | expression OR expression
    | NOT expression
    ;

// Expresión ternaria
ternary_expression
    : expression IF expression ELSE expression
    ;

// Expresión con type()
type_expression
    : TYPE OPEN_PARENTHESIS SELF CLOSE_PARENTHESIS OPEN_PARENTHESIS arg_list_prime CLOSE_PARENTHESIS
    {
        hasSelf = true;
    }
    ;

// Literales
list_literal
    : OPEN_BRACKET arg_list_prime CLOSE_BRACKET
    ;

tuple_literal
    : OPEN_PARENTHESIS arg_list_prime CLOSE_PARENTHESIS
    ;

// Statement genérico
statement
    : self_method_call
    | self_attribute_access
    | self_attribute_assign
    | method_call
    | super_call
    | PASS
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}