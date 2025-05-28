// RecursiveDescendant.cpp
#include "RecursiveDescendant.h"
#include <stdexcept>
#include <iostream>

RecursiveDescendant::RecursiveDescendant(TokenStream* stream) : Parser(stream) {
}

void RecursiveDescendant::parse() {
    try {
        program();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw e;
    }
}

void RecursiveDescendant::program() {
    preSkipStatements();
    elements();
    postSkipStatements();
}

void RecursiveDescendant::elements() {
    element();
    moreElements();
}

void RecursiveDescendant::moreElements() {
    if (isType(static_cast<int>(Tag::CLASS)) || 
        isType(static_cast<int>(Tag::DEF)) || 
        isType(static_cast<int>(Tag::CLASSMETHOD)) ||
        isType(static_cast<int>(Tag::PROPERTY)) ||
        isType(static_cast<int>(Tag::STATICMETHOD)) ||
        isType(static_cast<int>(Tag::ABSTRACTMETHOD))) { 
        element();
        moreElements();
    }
}

void RecursiveDescendant::element() {
    if (isType(static_cast<int>(Tag::CLASS))) {  
        classDefs();
    } else {
        methodDefs();
    }
}

void RecursiveDescendant::classDefs() {
    classDef();
    moreClassDefs();
}

void RecursiveDescendant::moreClassDefs() {
    if (isType(static_cast<int>(Tag::CLASS))) {
        classDef();
        moreClassDefs();
    }
}

void RecursiveDescendant::classDef() {
    match(static_cast<int>(Tag::CLASS));
    match(static_cast<int>(Tag::VARIABLE));
    inheritance();
    match(static_cast<int>(Tag::COLON));
    classSuite();
}

void RecursiveDescendant::inheritance() {
    if (isType(static_cast<int>(Tag::OPEN_PARENTHESIS))) {
        match(static_cast<int>(Tag::OPEN_PARENTHESIS));
        parentList();
        match(static_cast<int>(Tag::CLOSE_PARENTHESIS));
    }
}

void RecursiveDescendant::parentList() {
    match(static_cast<int>(Tag::VARIABLE));
    moreParents();
}

void RecursiveDescendant::moreParents() {
    if (isType(static_cast<int>(Tag::COMMA))) {
        match(static_cast<int>(Tag::COMMA));
        match(static_cast<int>(Tag::VARIABLE));
        moreParents();
    }
}

void RecursiveDescendant::methodDefs() {
    methodDef();
    moreMethodDefs();
}

void RecursiveDescendant::moreMethodDefs() {
    if (isType(static_cast<int>(Tag::DEF)) || 
        isType(static_cast<int>(Tag::CLASSMETHOD)) ||
        isType(static_cast<int>(Tag::PROPERTY)) ||
        isType(static_cast<int>(Tag::STATICMETHOD)) ||
        isType(static_cast<int>(Tag::ABSTRACTMETHOD))) {
        methodDef();
        moreMethodDefs();
    }
}

void RecursiveDescendant::methodDef() {
    if (isType(static_cast<int>(Tag::CLASSMETHOD))) {
        match(static_cast<int>(Tag::CLASSMETHOD));
        match(static_cast<int>(Tag::NEWLINE));
        methodDefCls();
    } else if (isType(static_cast<int>(Tag::PROPERTY))) {
        match(static_cast<int>(Tag::PROPERTY));
        match(static_cast<int>(Tag::NEWLINE));
        methodDefSelf();
    } else if (isType(static_cast<int>(Tag::ABSTRACTMETHOD))) {
        match(static_cast<int>(Tag::ABSTRACTMETHOD));
        match(static_cast<int>(Tag::NEWLINE));
        methodDefSelf();
    } else if (isType(static_cast<int>(Tag::STATICMETHOD))) {
        match(static_cast<int>(Tag::STATICMETHOD));
        match(static_cast<int>(Tag::NEWLINE));
        methodDefRaw();
    } else {
        methodDefSelf();
    }
}

void RecursiveDescendant::methodDefRaw() {
    match(static_cast<int>(Tag::DEF));
    methodName();
    match(static_cast<int>(Tag::OPEN_PARENTHESIS));
    paramList();
    match(static_cast<int>(Tag::CLOSE_PARENTHESIS));
    methodDefTail();
}

void RecursiveDescendant::methodDefSelf() {
    match(static_cast<int>(Tag::DEF));
    methodName();
    match(static_cast<int>(Tag::OPEN_PARENTHESIS));
    match(static_cast<int>(Tag::SELF));
    moreParams();
    match(static_cast<int>(Tag::CLOSE_PARENTHESIS));
    methodDefTail();
}

void RecursiveDescendant::methodDefCls() {
    match(static_cast<int>(Tag::DEF));
    methodName();
    match(static_cast<int>(Tag::OPEN_PARENTHESIS));
    match(static_cast<int>(Tag::CLS));
    moreParams();
    match(static_cast<int>(Tag::CLOSE_PARENTHESIS));
    methodDefTail();
}

void RecursiveDescendant::methodDefTail() {
    returnType();
    match(static_cast<int>(Tag::COLON));
    methodSuite();
}

void RecursiveDescendant::methodName() {
    if (isType(static_cast<int>(Tag::INIT))) {
        match(static_cast<int>(Tag::INIT));
    } else {
        match(static_cast<int>(Tag::VARIABLE));
    }
}

void RecursiveDescendant::paramList() {
    if (isType(static_cast<int>(Tag::VARIABLE))) {
        parameter();
        moreParams();
    }
}

void RecursiveDescendant::moreParams() {
    if (isType(static_cast<int>(Tag::COMMA))) {
        match(static_cast<int>(Tag::COMMA));
        parameter();
        moreParams();
    }
}

void RecursiveDescendant::parameter() {
    paramName();
    typeHint();
    defaultValue();
}

void RecursiveDescendant::paramName() {
    if (isType(static_cast<int>(Tag::VARIABLE))) {
        match(static_cast<int>(Tag::VARIABLE));
    } else {
        match(static_cast<int>(Tag::MULT));
        if (isType(static_cast<int>(Tag::VARIABLE))) {
            match(static_cast<int>(Tag::VARIABLE));
        } else {
            match(static_cast<int>(Tag::MULT));
            match(static_cast<int>(Tag::VARIABLE));
        }
    }
}

void RecursiveDescendant::typeHint() {
    if (isType(static_cast<int>(Tag::COLON))) {
        match(static_cast<int>(Tag::COLON));
        match(static_cast<int>(Tag::TYPE));
    }
}

void RecursiveDescendant::defaultValue() {
    if (isType(static_cast<int>(Tag::ASSIGN))) {
        match(static_cast<int>(Tag::ASSIGN));
        skipDefault();
    }
}

void RecursiveDescendant::returnType() {
    if (isType(static_cast<int>(Tag::ARROW))) {
        match(static_cast<int>(Tag::ARROW));
        match(static_cast<int>(Tag::TYPE));
    }
}

void RecursiveDescendant::methodSuite() {
    match(static_cast<int>(Tag::NEWLINE));
    match(static_cast<int>(Tag::INDENT));
    methodBody();
    match(static_cast<int>(Tag::DEDENT));
}

void RecursiveDescendant::classSuite() {
    match(static_cast<int>(Tag::NEWLINE));
    match(static_cast<int>(Tag::INDENT));
    classBody();
    match(static_cast<int>(Tag::DEDENT));
}

void RecursiveDescendant::classBody() {
    if (look && !isType(static_cast<int>(Tag::DEDENT))) {
        methodDefs();
    }
}

void RecursiveDescendant::methodBody() {
    if (look && !isType(static_cast<int>(Tag::DEDENT))) {
        skipStatement();
    }
}

void RecursiveDescendant::skipStatement() {
    int indentLevel = 1;
    
    while (look) {
        if (isType(static_cast<int>(Tag::INDENT))) {
            indentLevel++;
        } else if (isType(static_cast<int>(Tag::DEDENT))) {
            indentLevel--;
            if (indentLevel == 0) {
                return;
            }
        }
        move();
    }
}

void RecursiveDescendant::skipDefault() {
    while (look && !isType(static_cast<int>(Tag::COMMA)) &&
            !isType(static_cast<int>(Tag::CLOSE_PARENTHESIS))) {
        move();
    }
}

void RecursiveDescendant::preSkipStatements() {
    while (look && !isType(static_cast<int>(Tag::CLASS)) && 
            !isType(static_cast<int>(Tag::DEF)) &&
            !isType(static_cast<int>(Tag::CLASSMETHOD)) &&
            !isType(static_cast<int>(Tag::PROPERTY)) &&
            !isType(static_cast<int>(Tag::STATICMETHOD)) &&
            !isType(static_cast<int>(Tag::ABSTRACTMETHOD))) {
        move();
    }

}

void RecursiveDescendant::postSkipStatements() {
    while (look) {
        move();
    }
}