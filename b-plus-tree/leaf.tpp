#include "exceptions.hpp"
#include "internal.hpp"
#include "leaf.hpp"

#include <iostream>
#include <sstream>

template <typename K, typename T, unsigned O>
LeafType<K, T, O>::LeafType(NodeType<K, O> *parent) :
    fNext(nullptr), NodeType<K, O>(parent) {}

template <typename K, typename T, unsigned O>
LeafType<K, T, O>::~LeafType() {
    for (auto const &mapping : map) {
        delete mapping.second;
    }
}

template <typename K, typename T, unsigned O>
bool LeafType<K, T, O>::isLeaf() const {
    return true;
}

template <typename K, typename T, unsigned O>
LeafType<K, T, O> *LeafType<K, T, O>::next() const {
    return fNext;
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::setNext(LeafType<K, T, O> *aNext) {
    fNext = aNext;
}

template <typename K, typename T, unsigned O>
int LeafType<K, T, O>::size() const {
    return map.size();
}

template <typename K, typename T, unsigned O>
int LeafType<K, T, O>::minSize() const {
    return O / 2;
}

template <typename K, typename T, unsigned O>
int LeafType<K, T, O>::maxSize() const {
    return O - 1;
}

template <typename K, typename T, unsigned O>
std::string LeafType<K, T, O>::toString(bool aVerbose) const {
    std::ostringstream keyToTextConverter;
    if (aVerbose) {
        keyToTextConverter << "[" << std::hex << this << std::dec << "]<"
                           << map.size() << "> ";
    }
    bool first = true;
    for (auto const &mapping : map) {
        if (first) {
            first = false;
        } else {
            keyToTextConverter << " ";
        }
        keyToTextConverter << mapping.first;
    }
    if (aVerbose) {
        keyToTextConverter << "[" << std::hex << fNext << ">";
    }
    return keyToTextConverter.str();
}

template <typename K, typename T, unsigned O>
int LeafType<K, T, O>::createAndInsertRecord(K key, T value) {
    if (lookup(key) == nullptr) {
        insert(key, new T(value));
    }
    return map.size();
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::insert(K key, T *rec) {
    auto i = 0;
    while (i < map.size() && map[i].first < key) {
        ++i;
    }
    map.insert(MappingType(key, rec), i);
}

template <typename K, typename T, unsigned O>
T *LeafType<K, T, O>::lookup(K key) const {
    for (auto const &mapping : map) {
        if (mapping.first == key) {
            return mapping.second;
        }
    }
    return nullptr;
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::copyRangeStartingFrom(K key, Vector<Entry> &vec) {
    bool found = false;
    for (auto const &mapping : map) {
        if (mapping.first >= key) {
            found = true;
        }
        if (found) {
            vec.pushBack(makeTuple(mapping.first, *mapping.second, this));
        }
    }
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::copyRangeUntil(K key, Vector<Entry> &vec) {
    for (auto const &mapping : map) {
        if (mapping.first > key) {
            break;
        }
        vec.pushBack(makeTuple(mapping.first, *mapping.second, this));
    }
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::copyRange(Vector<Entry> &vec) {
    for (auto const &mapping : map) {
        vec.pushBack(makeTuple(mapping.first, *mapping.second, this));
    }
}

template <typename K, typename T, unsigned O>
int LeafType<K, T, O>::removeAndDeleteRecord(K key) {
    int i = 0;
    while (i < map.size() && map[i].first != key) {
        ++i;
    }
    if (i == map.size()) {
        throw RecordNotFound(key);
    }
    auto record = map[i];
    map.remove(i);
    delete record.second;
    return map.size();
}

template <typename K, typename T, unsigned O>
K const &LeafType<K, T, O>::firstKey() const {
    return map[0].first;
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::moveHalfTo(LeafType *aRecipient) {
    aRecipient->copyHalfFrom(map);
    size_t size = map.size();
    for (size_t i = minSize(); i < size; ++i) {
        map.popBack();
    }
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::copyHalfFrom(Vector<MappingType> &aMappings) {
    for (size_t i = minSize(); i < aMappings.size(); ++i) {
        map.pushBack(aMappings[i]);
    }
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::moveAllTo(LeafType *aRecipient, int) {
    aRecipient->copyAllFrom(map);
    map.clear();
    aRecipient->setNext(next());
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::copyAllFrom(Vector<MappingType> &aMappings) {
    for (auto const &mapping : aMappings) {
        map.pushBack(mapping);
    }
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::moveFirstToEndOf(LeafType *aRecipient) {
    aRecipient->copyLastFrom(map.front());
    map.erase(map.begin());
    static_cast<Internal *>(this->getParent())->setKeyAt(1, map.front().first);
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::copyLastFrom(MappingType aPair) {
    map.pushBack(aPair);
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::moveLastToFrontOf(LeafType *aRecipient,
                                          int aParentIndex) {
    aRecipient->copyFirstFrom(map.back(), aParentIndex);
    map.popBack();
}

template <typename K, typename T, unsigned O>
void LeafType<K, T, O>::copyFirstFrom(MappingType aPair, int aParentIndex) {
    map.insert(map.begin(), aPair);
    static_cast<Internal *>(this->getParent())
        ->setKeyAt(aParentIndex, map.front().first);
}
