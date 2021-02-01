#ifndef VECTOR_HPP__
#define VECTOR_HPP__

#include "utility.hpp"

#include <iostream>
template <class T>
class Vector {
    using ref = T &;
    using cref = const T &;
    class Iterator {
    private:
        Vector &v;
        int i;

    public:
        explicit Iterator(Vector &v, int i) : v(v), i(i) {}
        ref operator*() {
            return v[i];
        }
        cref operator*() const {
            return v[i];
        }
        T *operator->() {
            return &v[i];
        }
        T const *operator->() const {
            return &v[i];
        }
        Iterator &operator++() {
            ++i;
            return *this;
        }
        Iterator &operator--() {
            --i;
            return *this;
        }
        Iterator operator+(int x) const {
            return Iterator(v, x + i);
        }
        bool operator==(Iterator const &other) const {
            return i == other.i;
        }
        bool operator!=(Iterator const &other) const {
            return i != other.i;
        }
        int get() const {
            return i;
        }
    };
    class ConstIterator {
    private:
        Vector const &v;
        int i;

    public:
        explicit ConstIterator(Vector const &v, int i) : v(v), i(i) {}
        cref operator*() const {
            return v[i];
        }
        T const *operator->() const {
            return &v[i];
        }
        ConstIterator &operator++() {
            ++i;
            return *this;
        }
        ConstIterator &operator--() {
            --i;
            return *this;
        }
        ConstIterator operator+(int x) const {
            return ConstIterator(v, x + i);
        }
        bool operator==(ConstIterator const &other) const {
            return i == other.i;
        }
        bool operator!=(ConstIterator const &other) const {
            return i != other.i;
        }
    };

private:
    static const int factor = 2;
    T *buffer;
    int cap;
    int _end;
    bool inRange(int i) const {
        return i >= 0 && i < _end;
    }
    void reallocate(int newSize) {
        auto newBuffer = new T[newSize];
        for (int i = 0; i < _end; ++i) {
            newBuffer[i] = buffer[i];
        }
        delete[] buffer;
        buffer = newBuffer;
        cap = newSize;
    }
    bool full() const {
        return _end == cap;
    }

public:
    Vector() : buffer(new T[2]), cap(2), _end(0) {}
    Vector(int sz, T const &val = T()) : buffer(new T[sz]), cap(sz), _end(sz) {
        for (int i = 0; i < sz; ++i) {
            buffer[i] = val;
        }
    }
    Vector(const Vector &other) :
        buffer(new T[other.cap]), cap(other.cap), _end(other._end) {
        for (int i = 0; i < _end; ++i) {
            buffer[i] = other.buffer[i];
        }
    }
    Vector &operator=(const Vector &other) {
        if (this != &other) {
            delete[] buffer;

            buffer = new T[other._end];
            _end = other._end;
            cap = other.cap;

            for (int i = 0; i < _end; ++i) {
                buffer[i] = other[i];
            }
        }
        return *this;
    }
    ~Vector() {
        clear();
    }
    void pushBack(cref data) {
        insert(data, _end);
    }
    void popBack() {
        remove(_end - 1);
    }
    void insert(cref data, int index) {
        if (!inRange(index) && index != _end) {
            throw 0;
        }
        if (full()) {
            reallocate(cap * factor);
        }
        for (auto t = _end; t > index; --t) {
            buffer[t] = buffer[t - 1];
        }
        buffer[index] = data;
        ++_end;
    }
    void remove(int index) {
        if (!inRange(index)) {
            throw 0;
        }
        buffer[index].~T();
        for (auto t = index; t < _end - 1; t++) {
            buffer[t] = buffer[t + 1];
        }
        --_end;
    }
    int size() const {
        return _end;
    }
    bool empty() const {
        return _end == 0;
    }
    ref front() {
        return (*this)[0];
    }
    ref back() {
        return (*this)[_end - 1];
    }
    cref front() const {
        return (*this)[0];
    }
    cref back() const {
        return (*this)[_end - 1];
    }
    ref operator[](int index) {
        if (!inRange(index)) {
            throw 0;
        }
        return buffer[index];
    }
    cref operator[](int index) const {
        if (!inRange(index)) {
            throw 0;
        }
        return buffer[index];
    }
    void reserve(int newSize) {
        if (cap < newSize) {
            reallocate(newSize);
        }
    }
    void shrinkToFit() {
        reallocate(_end);
    }
    void clear() {
        delete[] buffer;
        buffer = new T[2];
        _end = 0;
        cap = 2;
    }
    void insert(const Iterator &it, cref data) {
        insert(data, it.get());
    }
    void erase(const Iterator &it) {
        remove(it.get());
    }
    Iterator begin() {
        return Iterator(*this, 0);
    }
    Iterator end() {
        return Iterator(*this, _end);
    }

    ConstIterator begin() const {
        return ConstIterator(*this, 0);
    }
    ConstIterator end() const {
        return ConstIterator(*this, _end);
    }
};

#endif // __VECTOR_HPP__
