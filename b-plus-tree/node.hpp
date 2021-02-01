#ifndef __NODE_HPP__
#define __NODE_HPP__

#include "utility.hpp"

#include <string>

template <typename K, unsigned O>
class NodeType {
public:
    explicit NodeType(NodeType *aParent);
    virtual ~NodeType();
    NodeType *getParent() const;
    void setParent(NodeType *aParent);
    bool isRoot() const;
    virtual bool isLeaf() const = 0;
    virtual int size() const = 0;
    virtual int minSize() const = 0;
    virtual int maxSize() const = 0;
    virtual std::string toString(bool aVerbose = false) const = 0;
    virtual K const &firstKey() const = 0;

private:
    NodeType *parent;
};

#include "node.tpp"

#endif // __NODE_HPP__
