#include "utility.hpp"

#include <iomanip>

std::string const &whitespace() {
    static std::string const ret = " \n\t";
    return ret;
}

Center::Center(std::string const &str, int width) :
    str(str), pad((width - str.length()) / 2) {}

std::ostream &operator<<(std::ostream &out, Center const &cen) {
    return out << std::setw(cen.pad) << "" << cen.str
               << std::setw(cen.pad + (cen.str.length() & 1)) << "";
}

template <>
std::string Sentinel() {
    return "";
}

bool in(char ch, std::string const &str) {
    for (auto i : str) {
        if (i == ch) {
            return true;
        }
    }
    return false;
}

Vector<std::string> tokenize(std::string const &str, std::string const &delim) {
    Vector<std::string> ret;
    int prev = 0;
    for (int i = 0; i < str.length(); ++i) {
        if (in(str.at(i), delim)) {
            ret.pushBack(str.substr(prev, i - prev));
            while (i < str.length() && in(str.at(i), delim)) {
                ++i;
            }
            prev = i;
        }
    }
    ret.pushBack(str.substr(prev));
    return ret;
}

void unquote(std::string &str) {
    if (str.front() == '\'')
        str.erase(str.begin());
    if (str.back() == '\'')
        str.pop_back();
}

void trim(std::string &str) {
    while (in(str.front(), whitespace())) {
        str.erase(str.begin());
    }
    while (in(str.back(), whitespace())) {
        str.pop_back();
    }
}

OutOfBounds::OutOfBounds(int index, int size) :
    msg("Tried to access " + std::to_string(index) + " in container of size " +
        std::to_string(size)) {}

char const *OutOfBounds::what() const noexcept {
    return msg.c_str();
}
