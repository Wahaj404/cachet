#include "item-not-found.hpp"

char const *ItemNotFound::what() const noexcept {
    static auto msg = "Item does not exist in hashmap";
    return msg;
}