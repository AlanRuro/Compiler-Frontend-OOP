%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"
#include "Word.h"
#include "Num.h"

// Function declarations
void yyerror(const char *s);
int yylex(void);
%}

%%
[ \t]+                  ; /* Skip whitespace */
\n                      { return NEWLINE; }
\r                      ; /* Skip carriage return */

"and"                   { return AND; }
"or"                    { return OR; }
"not"                   { return NOT; }
"True"                  { return TRUE; }
"False"                 { return FALSE; }
"None"                  { return NONE; }
"Any"                   { return ANY; }
"if"                    { return IF; }
"elif"                  { return ELIF; }
"else"                  { return ELSE; }
"for"                   { return FOR; }
"while"                 { return WHILE; }
"class"                 { return CLASS; }
"def"                   { return DEF; }
"init"                  { return INIT; }
"self"                  { return SELF; }
"super"                 { return SUPER; }
"return"                { return RETURN; }
"pass"                  { return PASS; }
"in"                    { return IN; }
"is"                    { return IS; }
"break"                 { return BREAK; }
"continue"              { return CONTINUE; }
"from"                  { return FROM; }
"import"                { return IMPORT; }

"int" | "float" | "str" | "list" | "dict" | "tuple" | "set" | "bool" { return TYPE; }

"@property" | "@staticmethod" | "@classmethod" | "@abstractmethod" { return POO_DECORATOR; }

"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MULT; }
"/"                     { return DIV; }
"%"                     { return MOD; }
"="                     { return ASSIGN; }
"=="                    { return LOGIC_OP; }
"!="                    { return LOGIC_OP; }
"<"                     { return LOGIC_OP; }
"<="                    { return LOGIC_OP; }
">"                     { return LOGIC_OP; }
">="                    { return LOGIC_OP; }
"->"                    { return ARROW; }

":"                     { return COLON; }
","                     { return COMMA; }
"."                     { return DOT; }
"("                     { return OPEN_PARENTHESIS; }
")"                     { return CLOSE_PARENTHESIS; }
"["                     { return OPEN_BRACKET; }
"]"                     { return CLOSE_BRACKET; }
"{"                     { return OPEN_BRACE; }
"}"                     { return CLOSE_BRACE; }

[0-9]+                  { return NUM; }
[0-9]+\.[0-9]+          { return NUM; }
@[a-zA-Z_][a-zA-Z0-9_]* { return DECORATOR; }
[a-zA-Z_][a-zA-Z0-9_]*  { return VARIABLE; }
\"([^\\\"]|\\.)*\"      { return STRING; }
\'([^\\\']|\\.)*\'      { return STRING; }

"#".*                  ; /* Skip comments */

.                       { printf("Unexpected character: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
} 