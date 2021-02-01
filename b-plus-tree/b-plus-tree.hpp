#ifndef __BPLUSTREE_HPP__
#define __BPLUSTREE_HPP__

#include "internal.hpp"
#include "leaf.hpp"
#include "node.hpp"
#include "tuple.hpp"
#include "vector.hpp"

template <typename K, typename T, unsigned O>
class BPlusTree {
    using Node = NodeType<K, O>;
    using Internal = InternalType<K, O>;
    using Leaf = LeafType<K, T, O>;
    using Entry = Tuple<K, T, Leaf *>;

public:
    BPlusTree();

    bool empty() const;

    void insert(K const &, T const &);

    T *get(K const &);

    void remove(K const &);

    void clear();
    Vector<Entry> range(K const &aStart, K const &aEnd) const;

private:
    void startNewTree(K const &aKey, T const &aValue);
    void insertIntoLeaf(K const &aKey, T const &aValue);
    void insertIntoParent(Node *aOldNode, K const &aKey, Node *aNewNode);
    template <typename N>
    N *split(N *aNode);
    void removeFromLeaf(K const &aKey);
    template <typename N>
    void coalesceOrRedistribute(N *aNode);
    template <typename N>
    void coalesce(N *aNeighborNode, N *aNode, Internal *aParent, int aIndex);
    template <typename N>
    void redistribute(N *aNeighborNode, N *aNode, Internal *aParent,
                      int aIndex);
    void adjustRoot();

public:
    Leaf *findLeaf(K const &aKey) const;
    Node *root;
};

#include "b-plus-tree.tpp"

#endif // __BPLUSTREE_HPP__
