#include "logical.hpp"

bool And::operator()(bool a, bool b) const {
    return a && b;
}

Logical *And::copy() const {
    return new And;
}

bool Or::operator()(bool a, bool b) const {
    return a || b;
}

Logical *Or::copy() const {
    return new Or;
}

bool Xor::operator()(bool a, bool b) const {
    return a ^ b;
}

Logical *Xor::copy() const {
    return new Xor;
}