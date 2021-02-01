#ifndef __LEAFNODE_HPP__
#define __LEAFNODE_HPP__

#include "node.hpp"
#include "pair.hpp"
#include "tuple.hpp"
#include "vector.hpp"

template <typename K, typename T, unsigned O>
class LeafType : public NodeType<K, O> {
    using MappingType = Pair<K, T *>;
    using Entry = Tuple<K, T, LeafType *>;
    using Node = NodeType<K, O>;
    using Internal = InternalType<K, O>;

public:
    explicit LeafType(Node * = nullptr);
    ~LeafType() override;
    bool isLeaf() const override;
    LeafType *next() const;
    void setNext(LeafType *);
    int size() const override;
    int minSize() const override;
    int maxSize() const override;
    int createAndInsertRecord(K, T);
    void insert(K, T *);
    T *lookup(K) const;
    int removeAndDeleteRecord(K);
    K const &firstKey() const override;
    void moveHalfTo(LeafType *);
    void moveAllTo(LeafType *, int);
    void moveFirstToEndOf(LeafType *);
    void moveLastToFrontOf(LeafType *, int);
    void copyRangeStartingFrom(K, Vector<Entry> &);
    void copyRangeUntil(K, Vector<Entry> &);
    void copyRange(Vector<Entry> &);
    std::string toString(bool = false) const override;

private:
    void copyHalfFrom(Vector<MappingType> &);
    void copyAllFrom(Vector<MappingType> &);
    void copyLastFrom(MappingType);
    void copyFirstFrom(MappingType, int);
    Vector<MappingType> map;
    LeafType *fNext;
};

#include "leaf.tpp"

#endif // __LEAFNODE_HPP__
