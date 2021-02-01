#include "comparator.hpp"

bool Less::operator()(std::string const &lhs, std::string const &rhs) const {
    return lhs < rhs;
}

Comparator *Less::copy() const {
    return new Less;
}

bool Equal::operator()(std::string const &lhs, std::string const &rhs) const {
    return lhs == rhs;
}

Comparator *Equal::copy() const {
    return new Equal;
}

bool Greater::operator()(std::string const &lhs, std::string const &rhs) const {
    return lhs > rhs;
}

Comparator *Greater::copy() const {
    return new Greater;
}

bool LessEqual::operator()(std::string const &lhs,
                           std::string const &rhs) const {
    return lhs <= rhs;
}

Comparator *LessEqual::copy() const {
    return new LessEqual;
}

bool NotEqual::operator()(std::string const &lhs,
                          std::string const &rhs) const {
    return lhs != rhs;
}

Comparator *NotEqual::copy() const {
    return new NotEqual;
}

bool GreaterEqual::operator()(std::string const &lhs,
                              std::string const &rhs) const {
    return lhs >= rhs;
}

Comparator *GreaterEqual::copy() const {
    return new GreaterEqual;
}