#include "exceptions.hpp"

#include <sstream>

template <typename K>
LeafNotFound<K>::LeafNotFound(K const &aKey) : fKey{aKey} {}

template <typename K>
const char *LeafNotFound<K>::what() const noexcept {
    std::ostringstream ss;
    ss << "Key not found in any leaf node:  ";
    ss << fKey;
    static std::string message;
    message = ss.str();
    return message.c_str();
}

// NodeNotFound::NodeNotFound(std::string const &searched,
//                            std::string const &containing) :
//     message(searched + " not found as a child of " + containing) {}

// const char *NodeNotFound::what() const noexcept {
//     return message.c_str();
// }

template <typename K>
RecordNotFound<K>::RecordNotFound(K const &aKey) : fKey{aKey} {}

template <typename K>
const char *RecordNotFound<K>::what() const noexcept {
    std::ostringstream ss;
    ss << "Record not found with key:  " << fKey;
    static std::string message;
    message = ss.str();
    return message.c_str();
}
