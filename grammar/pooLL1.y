%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int yylex();
void yyerror(const char *s);
extern FILE *yyin;

// Track OOP elements
bool hasClass = false;
bool hasInheritance = false;
bool hasInit = false;
bool hasSelf = false;
bool hasDecorator = false;
%}

// Token declarations
%token CLASS DEF SELF COLON COMMA DOT 
%token OPEN_PARENTHESIS CLOSE_PARENTHESIS
%token INDENT DEDENT NEWLINE VARIABLE SPECIAL_NAME
%token DECORATOR TYPE ASSIGN ARROW SUPER
%token EOF

%%

// Entry Point
program    : elements
           ;

// Top level elements
elements   : element elements
           | %empty
           ;

// Single element
element    : class_defs
           ;

// Class definitions and bodies (LL(1) version)
class_defs : class_def class_defs_prime
           | class_body class_defs_prime
           | %empty
           ;

class_defs_prime
           : class_def class_defs_prime
           | class_body class_defs_prime
           | %empty
           ;

// Class definition
class_def : CLASS VARIABLE inheritance COLON suite {
    hasClass = true;
} 
           ;

// Decorators (LL(1) version)
decorators: decorator NEWLINE decorators_prime
           | %empty
           ;

decorators_prime
           : decorator NEWLINE decorators_prime
           | %empty
           ;

decorator : DECORATOR {
    if (strcmp(yytext, "@property") == 0 ||
        strcmp(yytext, "@staticmethod") == 0 ||
        strcmp(yytext, "@classmethod") == 0 ||
        strcmp(yytext, "@abstractmethod") == 0) {
        hasDecorator = true;
    }
}
           ;

// Inheritance
inheritance: OPEN_PARENTHESIS parent_list CLOSE_PARENTHESIS {
    hasInheritance = true;
} 
           | %empty
           ;

// Parent list (LL(1) version)
parent_list: VARIABLE parent_list_prime
           ;

parent_list_prime
           : COMMA VARIABLE parent_list_prime
           | %empty
           ;

// Class body
class_body: NEWLINE INDENT class_body_elements DEDENT
           ;

// Class body elements (LL(1) version)
class_body_elements
           : class_body_element class_body_elements_prime
           ;

class_body_elements_prime
           : class_body_element class_body_elements_prime
           | %empty
           ;

class_body_element
           : method_def
           | skip_stmt
           ;

// Method definition
method_def: decorators DEF method_name OPEN_PARENTHESIS param_list CLOSE_PARENTHESIS return_type COLON suite
           ;

method_name: VARIABLE
           | SPECIAL_NAME {
    if (strcmp(yytext, "__init__") == 0) {
        hasInit = true;
    }
}
           ;

// Parameter list (LL(1) version)
param_list: parameter param_list_prime
           | %empty
           ;

param_list_prime
           : COMMA parameter param_list_prime
           | %empty
           ;

// Parameter with optional type and default value
parameter : SELF type_hint default_value {
    hasSelf = true;
}
           | VARIABLE type_hint default_value
           ;

type_hint : COLON TYPE
           | %empty
           ;

default_value
           : ASSIGN skip_stmt
           | %empty
           ;

return_type: ARROW TYPE
           | %empty
           ;

// Suite (method body)
suite     : NEWLINE INDENT suite_elements DEDENT
           | skip_stmt
           ;

// Suite elements (LL(1) version)
suite_elements
           : suite_element suite_elements_prime
           ;

suite_elements_prime
           : suite_element suite_elements_prime
           | %empty
           ;

suite_element
           : method_def
           | skip_stmt
           ;

// Skip statements (LL(1) version)
skip_stmt : skip_token skip_stmt_tail
           ;

skip_stmt_tail
           : skip_token skip_stmt_tail
           | NEWLINE
           | %empty
           ;

skip_token: VARIABLE
           | SPECIAL_NAME
           | SELF
           | SUPER
           | TYPE
           | COLON
           | COMMA
           | DOT
           | OPEN_PARENTHESIS
           | CLOSE_PARENTHESIS
           | ASSIGN
           | ARROW
           ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s input_file\n", argv[0]);
        exit(1);
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening input file");
        exit(1);
    }
    
    int result = yyparse();
    fclose(yyin);
    
    if (result == 0) {
        printf("OOP Analysis:\n");
        printf("Has class definition: %s\n", hasClass ? "Yes" : "No");
        printf("Has inheritance: %s\n", hasInheritance ? "Yes" : "No");
        printf("Has __init__ method: %s\n", hasInit ? "Yes" : "No");
        printf("Uses self: %s\n", hasSelf ? "Yes" : "No");
        printf("Has decorators: %s\n", hasDecorator ? "Yes" : "No");
        
        // Determine if it's OOP code
        if (hasClass && (hasInit || hasDecorator) && hasSelf) {
            printf("\nThis is valid OOP Python code!\n");
        } else {
            printf("\nThis is not valid OOP Python code.\n");
        }
    } else {
        printf("Parsing failed.\n");
    }
    
    return result;
}