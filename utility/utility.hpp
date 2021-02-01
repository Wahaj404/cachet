#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

#include "vector.hpp"

#include <string>

template <typename T>
void swap_(T &a, T &b) {
    T c = a;
    a = b;
    b = c;
}

template <typename T>
T Sentinel() {
    return std::numeric_limits<T>::min();
}

bool in(char ch, std::string const &str);

Vector<std::string> tokenize(std::string const &str, std::string const &delim);

void unquote(std::string &str);
void trim(std::string &str);
std::string const &whitespace();

template <typename T>
void printv(Vector<T> const &v) {
    for (auto const &i : v) {
        std::cerr << i << '\n';
    }
}

class Center {
private:
    std::string const &str;
    int pad;

public:
    Center(std::string const &, int);
    friend std::ostream &operator<<(std::ostream &, Center const &);
};

class OutOfBounds : public std::exception {
private:
    std::string msg;

public:
    OutOfBounds(int, int);
    char const *what() const noexcept override;
};

#endif // __UTILITY_HPP__
