#ifndef NUM_H
#define NUM_H

#include "Token.h"

class Num : public Token {
public:
    int value;
    Num(int v);
    std::string toString() override;
};

#endif // NUM_H