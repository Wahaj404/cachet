#ifndef __HASH_MAP_HPP__
#define __HASH_MAP_HPP__

#include "hash.hpp"
#include "item-not-found.hpp"
#include "pair.hpp"
#include "utility.hpp"
#include "vector.hpp"

template <typename K, typename T, unsigned B = 2>
class HashMap {
    struct Entry {
        K key;
        T value;
        bool active;

        Entry() : active(false) {}
    };

    struct Bucket {
        Entry entries[B];
    };

    int capacity;
    int sz;

    Bucket *table;

    Entry &entry(int i, int j) const {
        if (i < 0 || i >= capacity) {
            throw OutOfBounds(i, capacity);
        }
        if (j < 0 || j >= B) {
            throw OutOfBounds(j, B);
        }
        return (table + i)->entries[j];
    }
    int probe(int init, int iter) const {
        return (init + iter * iter) % capacity;
    }
    double loadFactor() const {
        return static_cast<double>(sz) / static_cast<double>(B * capacity);
    }
    void resize(int newCapacity);

public:
    HashMap();
    HashMap(HashMap<K, T, B> const &rhs);
    HashMap<K, T, B> &operator=(HashMap<K, T, B> const &rhs);
    ~HashMap();

    void insert(const K &key, const T &value);

    void remove(const K &key);

    T &get(const K &key) const;

    int size() const;
    Vector<K> keys() const;

    class Iterator {
    private:
        HashMap &map;
        int i;
        int j;

    public:
        Iterator(HashMap &map, int i, int j) : map(map), i(i), j(j) {}
        bool operator==(Iterator const &other) const {
            return i == other.i && j == other.j;
        }
        bool operator!=(Iterator const &other) const {
            return !(*this == other);
        }
        Iterator &operator++() {
            for (j = (j + 1) % B, i += (j == 0); i < map.capacity; ++i) {
                for (; j < B; ++j) {
                    if (map.entry(i, j).active) {
                        return *this;
                    }
                }
                j = 0;
            }

            return *this;
        }
        Pair<K, T> operator*() {
            return Pair<K, T>(map.entry(i, j).key, map.entry(i, j).value);
        }
        // Pair<K, T> *operator->() {
        //     return Pair<K, T>(map.entry(i, j).key, map.entry(i, j).value);
        // }
    };

    Iterator begin() const {
        for (int i = 0; i < capacity; ++i) {
            for (int j = 0; j < B; ++j) {
                if (entry(i, j).active) {
                    return Iterator(const_cast<HashMap &>(*this), i, j);
                }
            }
        }
        return end();
    }
    Iterator end() const {
        return Iterator(const_cast<HashMap &>(*this), capacity, 0);
    }
};

template <typename K, typename T, unsigned B>
HashMap<K, T, B>::HashMap() : capacity(5), sz(0), table(new Bucket[capacity]) {}

template <typename K, typename T, unsigned B>
HashMap<K, T, B>::HashMap(HashMap<K, T, B> const &rhs) {
    table = nullptr;
    *this = rhs;
}

template <typename K, typename T, unsigned B>
HashMap<K, T, B> &HashMap<K, T, B>::operator=(HashMap<K, T, B> const &rhs) {
    if (this != &rhs) {
        delete[] table;
        capacity = rhs.capacity;
        sz = rhs.sz;
        table = new Bucket[capacity];
        for (int i = 0; i < capacity; i++) {
            for (int j = 0; j < B; j++) {
                entry(i, j) = rhs.entry(i, j);
            }
        }
    }
    return *this;
}

template <typename K, typename T, unsigned B>
HashMap<K, T, B>::~HashMap() {
    delete[] table;
}

template <typename K, typename T, unsigned B>
void HashMap<K, T, B>::insert(K const &key, T const &value) {
    for (int pos = hash(key) % capacity, c = 0;; ++c) {
        int i = probe(pos, c);
        for (int j = 0; j < B; ++j) {
            if (!entry(i, j).active || entry(i, j).key == key) {
                if (!entry(i, j).active) {
                    ++sz;
                }
                entry(i, j).key = key;
                entry(i, j).value = value;
                entry(i, j).active = true;
                if (loadFactor() > 0.6) {
                    resize(nextPrime(capacity));
                }
                return;
            }
        }
    }
}

template <typename K, typename T, unsigned B>
void HashMap<K, T, B>::remove(const K &key) {
    for (int pos = hash(key) % capacity, i = 0; i < capacity / 2; ++i) {
        int cur = probe(pos, i);
        for (int j = 0; j < B; ++j) {
            if (entry(cur, j).active && entry(cur, j).key == key) {
                entry(cur, j).active = false;
                --sz;
                return;
            }
        }
    }
    throw ItemNotFound();
}

template <typename K, typename T, unsigned B>
T &HashMap<K, T, B>::get(K const &key) const {
    for (int pos = hash(key) % capacity, i = 0; i < capacity / 2; ++i) {
        int cur = probe(pos, i);
        for (int j = 0; j < B; ++j) {
            if (entry(cur, j).active && entry(cur, j).key == key) {
                return entry(cur, j).value;
            }
        }
    }
    throw ItemNotFound();
}

template <typename K, typename T, unsigned B>
void HashMap<K, T, B>::resize(int newCapacity) {
    int cap = capacity;
    auto old = table;

    capacity = newCapacity;
    sz = 0;
    table = new Bucket[capacity];
    for (int i = 0; i < cap; ++i) {
        for (int j = 0; j < B; ++j) {
            if (old[i].entries[j].active) {
                insert(old[i].entries[j].key, old[i].entries[j].value);
            }
        }
    }
    delete[] old;
}

template <typename K, typename T, unsigned B>
int HashMap<K, T, B>::size() const {
    return sz;
}

template <typename K, typename T, unsigned B>
Vector<K> HashMap<K, T, B>::keys() const {
    Vector<K> ret;
    for (int i = 0; i < capacity; i++) {
        for (int j = 0; j < B; j++) {
            if (entry(i, j).active) {
                ret.pushBack(entry(i, j).value);
            }
        }
    }
    return ret;
}

#include "hash-map.tpp"

#endif // __HASH_MAP_HPP__
