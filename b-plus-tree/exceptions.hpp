#ifndef __EXCEPTIONS_HPP__
#define __EXCEPTIONS_HPP__

#include <exception>
#include <string>

template <typename K>
class LeafNotFound : public std::exception {
public:
    explicit LeafNotFound(K const &);
    const char *what() const noexcept override;

private:
    K fKey;
};

// class NodeNotFound : public std::exception {
// public:
//     NodeNotFound(std::string const &, std::string const &);
//     const char *what() const noexcept override;

// private:
//     std::string message;
// };

template <typename K>
class RecordNotFound : public std::exception {
public:
    explicit RecordNotFound(K const &);
    const char *what() const noexcept override;

private:
    K fKey;
};

#include "exceptions.tpp"

#endif // __EXCEPTIONS_HPP__
