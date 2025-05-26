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
- `CLASS`: Class definition keyword
- `DEF`: Method definition keyword
- `SELF`: Self parameter
- `SUPER`: Super keyword
- `DECORATOR`: Decorator symbol (@)
- `COLON`: Colon symbol (:)
- `COMMA`: Comma symbol (,)
- `OPEN_PARENTHESIS`: Opening parenthesis (()
- `CLOSE_PARENTHESIS`: Closing parenthesis ())
- `ASSIGN`: Assignment operator (=)
- `ARROW`: Return type arrow (->)
- `NEWLINE`: Line break
- `INDENT`: Indentation increase
- `DEDENT`: Indentation decrease
- `TYPE`: Type annotation
- `VARIABLE`: Variable name
- `SPECIAL_NAME`: Special method name (e.g., __init__)

## Notes
1. The parser uses recursive descent parsing with explicit method calls
2. Expression parsing is simplified - expressions are skipped rather than fully parsed
3. The grammar focuses on class and method structure rather than detailed expression handling
4. Indentation is explicitly handled to support Python-like block structure
5. The parser is designed for static analysis of OOP code structure rather than full language implementation

## Example
```python
class MyClass(ParentClass):
    @property
    def my_method(self, param: int = 0) -> str:
        self.attribute = value
        return result
``` 