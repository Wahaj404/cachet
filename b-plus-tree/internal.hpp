#ifndef __INTERNALNODE_HPP__
#define __INTERNALNODE_HPP__

#include "node.hpp"
#include "pair.hpp"
#include "vector.hpp"

template <typename K, unsigned O>
class InternalType : public NodeType<K, O> {
    using Node = NodeType<K, O>;
    using MappingType = Pair<K, Node *>;

public:
    explicit InternalType(Node *parent = nullptr);
    ~InternalType() override;
    bool isLeaf() const override;
    int size() const override;
    int minSize() const override;
    int maxSize() const override;
    K const &keyAt(int aIndex) const;
    void setKeyAt(int aIndex, K const &aKey);
    Node *firstChild() const;
    void populateNewRoot(Node *oldNode, K const &key, Node *newNode);
    int insertNodeAfter(Node *oldNode, K const &key, Node *newNode);
    void remove(int aIndex);
    Node *removeAndReturnOnlyChild();
    K replaceAndReturnFirstKey();
    void moveHalfTo(InternalType *aRecipient);
    void moveAllTo(InternalType *aRecipient, int aIndexInParent);
    void moveFirstToEndOf(InternalType *aRecipient);
    void moveLastToFrontOf(InternalType *aRecipient, int aParentIndex);
    Node *lookup(K const &aKey) const;
    int nodeIndex(Node *aNode) const;
    Node *neighbor(int aIndex) const;
    std::string toString(bool aVerbose = false) const override;
    K const &firstKey() const override;

private:
    void copyHalfFrom(Vector<MappingType> &aMappings);
    void copyAllFrom(Vector<MappingType> &aMappings);
    void copyLastFrom(MappingType aPair);
    void copyFirstFrom(MappingType aPair, int aParentIndex);
    Vector<MappingType> map;
};

#include "internal.tpp"

#endif // __INTERNALNODE_HPP__
