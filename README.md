# Grammar Specification for Recursive Descent Parser

This document describes the grammar specification implemented in `RecursiveDescendant.cpp`. The parser is designed to recognize Python-like OOP code structures.

## Grammar Rules

### Program Structure
```
program → preSkipStatements elements postSkipStatements
elements → element moreElements
moreElements → element moreElements | ε
element → classDefs | methodDefs
```

### Class Definitions
```
classDefs → classDef moreClassDefs
moreClassDefs → classDef moreClassDefs | ε
classDef → CLASS VARIABLE inheritance COLON classSuite
inheritance → (parentList) | ε
parentList → VARIABLE moreParents
moreParents → , VARIABLE moreParents | ε
```

### Method Definitions
```
methodDefs → methodDef moreMethodDefs
moreMethodDefs → methodDef moreMethodDefs | ε
methodDef → decorators DEF methodName (paramList) returnType COLON methodSuite
            | decorators DEF methodName (SELF moreParams) returnType COLON methodSuite
            | DEF methodName (SELF moreParams) returnType COLON methodSuite
decorators → decorator moreDecorators
moreDecorators → decorator moreDecorators | ε
decorator → POO_DECORATOR NEWLINE
methodName → SPECIAL_NAME | VARIABLE
paramList → parameter moreParams | ε
moreParams → , parameter moreParams | ε
parameter → paramName typeHint defaultValue
paramName → VARIABLE | * VARIABLE | ** VARIABLE
typeHint → : TYPE | ε
defaultValue → = skipDefault | ε
returnType → -> TYPE | ε
```

### Suite and Statements
```
classSuite → NEWLINE INDENT classBody DEDENT
methodSuite → NEWLINE INDENT methodBody DEDENT
methodBody → skipStatement methodBody | ε
classBody → methodDefs methodBody | ε
preSkipStatements → (any tokens until class or method def)
skipStatement → (any tokens until DEDENT)
skipDefault → (any tokens until ',' or ')' )
postSkipStatements → (any tokens until EOF)
```

## Token Types
The grammar recognizes the following token types:

### Keywords
- `CLASS`: Class definition keyword
- `DEF`: Method definition keyword
- `SELF`: Self parameter
- `INIT`: Special method name (__init__)
- `SUPER`: Super keyword
- `NONE`: None value
- `ANY`: Any type
- `TRUE`: Boolean true
- `FALSE`: Boolean false
- `RETURN`: Return statement
- `PASS`: Pass statement
- `IN`: In operator
- `IS`: Is operator
- `IF`: If statement
- `ELIF`: Else if statement
- `ELSE`: Else statement
- `FOR`: For loop
- `WHILE`: While loop
- `BREAK`: Break statement
- `CONTINUE`: Continue statement
- `FROM`: From import
- `IMPORT`: Import statement

### OOP Specific
- `DECORATOR`: Decorator symbol (@)
- `POO_DECORATOR`: OOP-specific decorator (@property, @staticmethod, @classmethod, @abstractmethod)

### Punctuation
- `COLON`: Colon symbol (:)
- `COMMA`: Comma symbol (,)
- `DOT`: Dot symbol (.)
- `OPEN_PARENTHESIS`: Opening parenthesis (()
- `CLOSE_PARENTHESIS`: Closing parenthesis ())
- `OPEN_BRACKET`: Opening bracket ([)
- `CLOSE_BRACKET`: Closing bracket (])
- `OPEN_BRACE`: Opening brace ({)
- `CLOSE_BRACE`: Closing brace (})
- `ASSIGN`: Assignment operator (=)
- `ARROW`: Return type arrow (->)

### Indentation
- `INDENT`: Indentation increase
- `DEDENT`: Indentation decrease
- `NEWLINE`: Line break

### Types and Variables
- `TYPE`: Type annotation (int, float, str, etc.)
- `VARIABLE`: Variable name
- `NUM`: Numeric literal
- `STRING`: String literal

### Operators
- `LOGIC_OP`: Logical operators (==, !=, <, >, <=, >=)
- `PLUS`: Addition operator (+)
- `MINUS`: Subtraction operator (-)
- `MULT`: Multiplication operator (*)
- `DIV`: Division operator (/)
- `MOD`: Modulo operator (%)
- `AND`: Logical AND
- `OR`: Logical OR
- `NOT`: Logical NOT

## Notes
1. The parser uses recursive descent parsing with explicit method calls
2. Expression parsing is simplified - expressions are skipped rather than fully parsed
3. The grammar focuses on class and method structure rather than detailed expression handling
4. Indentation is explicitly handled to support Python-like block structure
5. The parser is designed for static analysis of OOP code structure rather than full language implementation
6. DEDENT tokens are made with semantic logic in the scanner and handled in the parser

## Example
```python
class MyClass(ParentClass):
    @property
    def my_method(self, param: int = 0) -> str:
        self.attribute = value
        return result
``` 