#include "Num.h"

Num::Num(int v) : Token(static_cast<int>(Tag::NUM)) {
    this->value = v;
}

std::string Num::toString() {
    return std::to_string(value);
}