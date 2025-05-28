# Grammar Specification for Recursive Descent Parser

This document describes the grammar specification implemented in `RecursiveDescendant.cpp`. The parser is designed to recognize Python-like OOP code structures.

## Usage

### Prerequisites
- C++17 compatible compiler (g++ recommended)
- Python 3.x or Python 2.x
- Make

### Building the Project
1. Clone the repository:
```bash
git clone <repository-url>
cd <repository-name>
```

2. Build the project:
```bash
make
```
This will create the executable in `build/bin/main`.

### Running the Parser
1. To parse a Python file:
```bash
make test FILE=path/to/your/file.py
```
or directly:
```bash
./build/bin/main path/to/your/file.py
```

2. To process the dataset:
```bash
make dataset
```
This will run the Python script to process the dataset in the `scripts` directory.

### Cleaning
To clean build files:
```bash
make clean
```

### Project Structure
```
.
├── include/           # Header files
│   ├── Lexer.h       # Lexical analyzer
│   ├── Parser.h      # Base parser class
│   ├── RecursiveDescendant.h  # Recursive descent parser
│   ├── Token.h       # Token definitions
│   └── Word.h        # Word token class
├── src/              # Source files
│   ├── Lexer.cpp     # Lexer implementation
│   ├── Parser.cpp    # Parser implementation
│   └── RecursiveDescendant.cpp  # Parser implementation
├── scripts/          # Python scripts
│   └── process_dataset.py  # Dataset processing
├── build/            # Build directory (created by make)
│   ├── bin/         # Executables
│   └── obj/         # Object files
├── Makefile         # Build configuration
└── README.md        # This file
```

### Output
The parser will analyze Python files and:
1. Validate the OOP structure (classes and methods)
2. Check for proper indentation
3. Verify method decorators
4. Validate parameter lists and type hints
5. Report any syntax errors found

### Error Messages
The parser provides detailed error messages including:
- Line and column numbers
- Expected vs found tokens
- Syntax errors
- Indentation errors

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
methodDef → CLASSMETHOD NEWLINE methodDefCls 
            | PROPERTY NEWLINE methodDefSelf 
            | STATICMETHOD NEWLINE methodDefRaw
            | ABSTRACTMETHOD NEWLINE methodDefSelf 
            | methodDefSelf
methodDefRaw → DEF methodName (paramList) methodDefTail
methodDefSelf → DEF methodName (SELF moreParams) methodDefTail
methodDefCls → DEF methodName (CLS moreParams) methodDefTail
methodDefTail → returnType COLON methodSuite
methodName → INIT | VARIABLE
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
- `CLS`: Cls parameter
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
- `PROPERTY`: OOP-specific decorator @property
- `STATICMETHOD`: OOP-specific decorator @staticmethod
- `CLASSMETHOD`: OOP-specific decorator @classmethod
- `ABSTRACTMETHOD`: OOP-specific decorator @abstractmethod

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
7. The parser does not validate the statements in method bodies, nor the statements before and after the elements of OOP code
8. For the OOP-specific decorators, they have to be next to the method definition
9. If there are many elements of OOP code separated by normal statements, just the first element found will be validated

## Example
```python
class MyClass(ParentClass):
    @property
    def my_method(self, param: int = 0) -> str:
        self.attribute = value
        return result
``` 