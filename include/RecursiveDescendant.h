#ifndef RECURSIVE_DESCENDANT_H
#define RECURSIVE_DESCENDANT_H

#include "Parser.h"

class RecursiveDescendant : public Parser {
public:
    RecursiveDescendant(TokenStream* stream);
    void parse();

private:
    // Grammar
    void program();
    void elements();
    void moreElements();
    void element();
    void classDefs();
    void moreClassDefs();
    void classDef();
    void inheritance();
    void parentList();
    void moreParents();
    void methodDefs();
    void moreMethodDefs();
    void methodDef();
    void methodDefRaw();
    void methodDefSelf();
    void methodDefCls();
    void methodDefTail();
    void methodName();
    void paramList();
    void parameter();
    void paramName();
    void moreParams();
    void typeHint();
    void defaultValue();
    void returnType();
    void methodSuite();
    void methodBody();
    void classSuite();
    void classBody();
    
    // Skip any statement we don't care about 
    void skipStatement(); 

    void skipDefault();
    void preSkipStatements();
    void postSkipStatements();
};

#endif