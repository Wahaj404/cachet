#include "node.hpp"

template <typename K, unsigned O>
NodeType<K, O>::NodeType(NodeType *parent) : parent(parent) {}

template <typename K, unsigned O>
NodeType<K, O>::~NodeType() {}

template <typename K, unsigned O>
NodeType<K, O> *NodeType<K, O>::getParent() const {
    return parent;
}

template <typename K, unsigned O>
void NodeType<K, O>::setParent(NodeType *parent) {
    this->parent = parent;
}

template <typename K, unsigned O>
bool NodeType<K, O>::isRoot() const {
    return parent == nullptr;
}
