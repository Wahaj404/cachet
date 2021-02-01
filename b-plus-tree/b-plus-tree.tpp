#include "b-plus-tree.hpp"

#include <iostream>
#include <string>

template <typename K, typename T, unsigned O>
BPlusTree<K, T, O>::BPlusTree() : root(nullptr) {}

template <typename K, typename T, unsigned O>
bool BPlusTree<K, T, O>::empty() const {
    return root == nullptr;
}

template <typename K, typename T, unsigned O>
T *BPlusTree<K, T, O>::get(K const &key) {
    return findLeaf(key)->lookup(key);
}

template <typename K, typename T, unsigned O>
void BPlusTree<K, T, O>::insert(K const &key, T const &value) {
    if (empty()) {
        startNewTree(key, value);
    } else {
        insertIntoLeaf(key, value);
    }
}

template <typename K, typename T, unsigned O>
void BPlusTree<K, T, O>::startNewTree(K const &key, T const &value) {
    root = new Leaf;
    static_cast<Leaf *>(root)->createAndInsertRecord(key, value);
}

template <typename K, typename T, unsigned O>
void BPlusTree<K, T, O>::insertIntoLeaf(K const &key, T const &value) {
    auto leafNode = findLeaf(key);
    if (!leafNode) {
        throw LeafNotFound(key);
    }
    auto record = leafNode->lookup(key);
    if (record) {
        *record = value;
        return;
    }
    int newSize = leafNode->createAndInsertRecord(key, value);
    if (newSize > leafNode->maxSize()) {
        auto newLeaf = split(leafNode);
        newLeaf->setNext(leafNode->next());
        leafNode->setNext(newLeaf);
        K newKey = newLeaf->firstKey();
        insertIntoParent(leafNode, newKey, newLeaf);
    }
}

template <typename K, typename T, unsigned O>
void BPlusTree<K, T, O>::insertIntoParent(Node *old, K const &key, Node *new_) {
    auto parent = static_cast<Internal *>(old->getParent());
    if (parent == nullptr) {
        root = new Internal;
        parent = static_cast<Internal *>(root);
        old->setParent(parent);
        new_->setParent(parent);
        parent->populateNewRoot(old, key, new_);
    } else {
        int newSize = parent->insertNodeAfter(old, key, new_);
        if (newSize > parent->maxSize()) {
            auto newNode = split(parent);
            K newKey = newNode->replaceAndReturnFirstKey();
            insertIntoParent(parent, newKey, newNode);
        }
    }
}

template <typename K, typename T, unsigned O>
template <typename N>
N *BPlusTree<K, T, O>::split(N *aNode) {
    N *newNode = new N(aNode->getParent());
    aNode->moveHalfTo(newNode);
    return newNode;
}

// REMOVAL
template <typename K, typename T, unsigned O>
void BPlusTree<K, T, O>::remove(K const &key) {
    if (!empty()) {
        removeFromLeaf(key);
    }
}

template <typename K, typename T, unsigned O>
void BPlusTree<K, T, O>::removeFromLeaf(K const &key) {
    auto leafNode = findLeaf(key);
    if (!leafNode) {
        return;
    }
    if (!leafNode->lookup(key)) {
        return;
    }
    int newSize = leafNode->removeAndDeleteRecord(key);
    if (newSize < leafNode->minSize()) {
        coalesceOrRedistribute(leafNode);
    }
}

template <typename K, typename T, unsigned O>
template <typename N>
void BPlusTree<K, T, O>::coalesceOrRedistribute(N *aNode) {
    if (aNode->isRoot()) {
        adjustRoot();
        return;
    }
    auto parent = static_cast<Internal *>(aNode->getParent());
    int indexOfNodeInParent = parent->nodeIndex(aNode);
    int neighborIndex =
        (indexOfNodeInParent == 0) ? 1 : indexOfNodeInParent - 1;
    N *neighborNode = static_cast<N *>(parent->neighbor(neighborIndex));
    if (aNode->size() + neighborNode->size() <= neighborNode->maxSize()) {
        coalesce(neighborNode, aNode, parent, indexOfNodeInParent);
    } else {
        redistribute(neighborNode, aNode, parent, indexOfNodeInParent);
    }
}

template <typename K, typename T, unsigned O>
template <typename N>
void BPlusTree<K, T, O>::coalesce(N *aNeighborNode, N *aNode, Internal *aParent,
                                  int aIndex) {
    if (aIndex == 0) {
        std::swap(aNode, aNeighborNode);
        aIndex = 1;
    }
    aNode->moveAllTo(aNeighborNode, aIndex);
    aParent->remove(aIndex);
    if (aParent->size() < aParent->minSize()) {
        coalesceOrRedistribute(aParent);
    }
    delete aNode;
}

template <typename K, typename T, unsigned O>
template <typename N>
void BPlusTree<K, T, O>::redistribute(N *aNeighborNode, N *aNode,
                                      Internal *aParent, int aIndex) {
    if (aIndex == 0) {
        aNeighborNode->moveFirstToEndOf(aNode);
    } else {
        aNeighborNode->moveLastToFrontOf(aNode, aIndex);
    }
}
template <typename K, typename T, unsigned O>
void BPlusTree<K, T, O>::adjustRoot() {
    if (!root->isLeaf() && root->size() == 1) {
        auto discardedNode = static_cast<Internal *>(root);
        root = static_cast<Internal *>(root)->removeAndReturnOnlyChild();
        root->setParent(nullptr);
        delete discardedNode;
    } else if (!root->size()) {
        delete root;
        root = nullptr;
    }
}

// UTILITIES AND PRINTING
template <typename K, typename T, unsigned O>
LeafType<K, T, O> *BPlusTree<K, T, O>::findLeaf(K const &key) const {
    if (empty()) {
        return nullptr;
    }
    auto node = root;

    while (!node->isLeaf()) {
        auto internalNode = static_cast<Internal *>(node);
        node = internalNode->lookup(key);
    }
    return static_cast<Leaf *>(node);
}

template <typename K, typename T, unsigned O>
void BPlusTree<K, T, O>::clear() {
    if (root->isLeaf()) {
        delete static_cast<Leaf *>(root);
    } else {
        delete static_cast<Internal *>(root);
    }
    root = nullptr;
}

template <typename K, typename T, unsigned O>
Vector<typename BPlusTree<K, T, O>::Entry>
BPlusTree<K, T, O>::range(K const &to, K const &from) const {
    auto start = findLeaf(to);
    auto end = findLeaf(from);
    Vector<Entry> entries;
    if (to <= from && start != nullptr && end != nullptr) {
        if (start == end) {
            start->copyRangeUntil(from, entries);
        } else {
            start->copyRangeStartingFrom(to, entries);
            start = start->next();
            while (start != end) {
                start->copyRange(entries);
                start = start->next();
            }
            start->copyRangeUntil(from, entries);
        }
    }
    return entries;
}
