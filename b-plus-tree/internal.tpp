#include "exceptions.hpp"
#include "internal.hpp"

#include <iostream>
#include <sstream>

template <typename K, unsigned O>
InternalType<K, O>::InternalType(Node *aParent) : NodeType<K, O>(aParent) {}

template <typename K, unsigned O>
InternalType<K, O>::~InternalType() {
    for (auto mapping : map) {
        delete mapping.second;
    }
}

template <typename K, unsigned O>
bool InternalType<K, O>::isLeaf() const {
    return false;
}

template <typename K, unsigned O>
int InternalType<K, O>::size() const {
    return static_cast<int>(map.size());
}

template <typename K, unsigned O>
int InternalType<K, O>::minSize() const {
    return (O + 1) / 2;
}

template <typename K, unsigned O>
int InternalType<K, O>::maxSize() const {
    return O;
}

template <typename K, unsigned O>
K const &InternalType<K, O>::keyAt(int aIndex) const {
    return map[aIndex].first;
}

template <typename K, unsigned O>
void InternalType<K, O>::setKeyAt(int aIndex, K const &aKey) {
    map[aIndex].first = aKey;
}

template <typename K, unsigned O>
NodeType<K, O> *InternalType<K, O>::firstChild() const {
    return map.front().second;
}

template <typename K, unsigned O>
void InternalType<K, O>::populateNewRoot(NodeType<K, O> *aOldNode,
                                         K const &aNewKey,
                                         NodeType<K, O> *aNewNode) {
    map.pushBack(Pair(Sentinel<K>(), aOldNode));
    map.pushBack(Pair(aNewKey, aNewNode));
}

template <typename K, unsigned O>
int InternalType<K, O>::insertNodeAfter(NodeType<K, O> *aOldNode,
                                        K const &aNewKey,
                                        NodeType<K, O> *aNewNode) {
    auto iter = map.begin();
    for (; iter != map.end() && iter->second != aOldNode; ++iter)
        ;
    map.insert(iter + 1, Pair(aNewKey, aNewNode));
    return size();
}

template <typename K, unsigned O>
void InternalType<K, O>::remove(int aIndex) {
    map.erase(map.begin() + aIndex);
}

template <typename K, unsigned O>
NodeType<K, O> *InternalType<K, O>::removeAndReturnOnlyChild() {
    NodeType<K, O> *firstChild = map.front().second;
    map.popBack();
    return firstChild;
}

template <typename K, unsigned O>
K InternalType<K, O>::replaceAndReturnFirstKey() {
    K newKey = map[0].first;
    map[0].first = Sentinel<K>();
    return newKey;
}

template <typename K, unsigned O>
void InternalType<K, O>::moveHalfTo(InternalType<K, O> *aRecipient) {
    aRecipient->copyHalfFrom(map);
    size_t size = map.size();
    for (size_t i = minSize(); i < size; ++i) {
        map.popBack();
    }
}

template <typename K, unsigned O>
void InternalType<K, O>::copyHalfFrom(Vector<MappingType> &aMappings) {
    for (size_t i = minSize(); i < aMappings.size(); ++i) {
        aMappings[i].second->setParent(this);
        map.pushBack(aMappings[i]);
    }
}

template <typename K, unsigned O>
void InternalType<K, O>::moveAllTo(InternalType<K, O> *aRecipient,
                                   int aIndexInParent) {
    map[0].first = static_cast<InternalType<K, O> *>(this->getParent())
                       ->keyAt(aIndexInParent);
    aRecipient->copyAllFrom(map);
    map.clear();
}

template <typename K, unsigned O>
void InternalType<K, O>::copyAllFrom(Vector<MappingType> &aMappings) {
    for (auto mapping : aMappings) {
        mapping.second->setParent(this);
        map.pushBack(mapping);
    }
}

template <typename K, unsigned O>
void InternalType<K, O>::moveFirstToEndOf(InternalType<K, O> *aRecipient) {
    aRecipient->copyLastFrom(map.front());
    map.erase(map.begin());
    static_cast<InternalType<K, O> *>(this->getParent())
        ->setKeyAt(1, map.front().first);
}

template <typename K, unsigned O>
void InternalType<K, O>::copyLastFrom(MappingType aPair) {
    map.pushBack(aPair);
    map.back().first = map.back().second->firstKey();
    map.back().second->setParent(this);
}

template <typename K, unsigned O>
void InternalType<K, O>::moveLastToFrontOf(InternalType<K, O> *aRecipient,
                                           int aParentIndex) {
    aRecipient->copyFirstFrom(map.back(), aParentIndex);
    map.popBack();
}

template <typename K, unsigned O>
void InternalType<K, O>::copyFirstFrom(MappingType aPair, int aParentIndex) {
    map.front().first = static_cast<InternalType<K, O> *>(this->getParent())
                            ->keyAt(aParentIndex);
    map.insert(map.begin(), aPair);
    map.front().first = Sentinel<K>();
    map.front().second->setParent(this);
    static_cast<InternalType<K, O> *>(this->getParent())
        ->setKeyAt(aParentIndex, map.front().first);
}

template <typename K, unsigned O>
NodeType<K, O> *InternalType<K, O>::lookup(K const &aKey) const {
    auto locator = map.begin();
    auto end = map.end();
    while (locator != end && aKey >= locator->first) {
        ++locator;
    }
    --locator;
    return locator->second;
}

template <typename K, unsigned O>
int InternalType<K, O>::nodeIndex(NodeType<K, O> *aNode) const {
    for (size_t i = 0; i < size(); ++i) {
        if (map[i].second == aNode) {
            return static_cast<int>(i);
        }
    }
    throw LeafNotFound(K());
}

template <typename K, unsigned O>
NodeType<K, O> *InternalType<K, O>::neighbor(int aIndex) const {
    return map[aIndex].second;
}

template <typename K, unsigned O>
std::string InternalType<K, O>::toString(bool aVerbose) const {
    if (map.empty()) {
        return "";
    }
    std::ostringstream keyToTextConverter;
    if (aVerbose) {
        keyToTextConverter << "[" << std::hex << this << std::dec << "]<"
                           << map.size() << "> ";
    }
    auto entry = aVerbose ? map.begin() : map.begin() + 1;
    auto end = map.end();
    bool first = true;
    while (entry != end) {
        if (first) {
            first = false;
        } else {
            keyToTextConverter << " ";
        }
        keyToTextConverter << std::dec << entry->first;
        if (aVerbose) {
            keyToTextConverter << "(" << std::hex << entry->second << std::dec
                               << ")";
        }
        ++entry;
    }
    return keyToTextConverter.str();
}

template <typename K, unsigned O>
K const &InternalType<K, O>::firstKey() const {
    return map[1].first;
}
