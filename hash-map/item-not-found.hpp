#ifndef __ITEM_NOT_FOUND_HPP__
#define __ITEM_NOT_FOUND_HPP__

#include <exception>

class ItemNotFound : public std::exception {
public:
    char const *what() const noexcept override;
};

#endif // __ITEM_NOT_FOUND_HPP__
