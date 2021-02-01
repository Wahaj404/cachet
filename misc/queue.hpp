#ifndef __QUEUE_HPP__
#define __QUEUE_HPP__

#include <iomanip>
#include <iostream>

template <class T>
class Queue {
    using ref = T &;
    using cref = const T &;

private:
    static const int factor = 2;
    T *buffer;
    int start;
    int cnt;
    int size;

public:
    Queue() : buffer(new T[5]), start(0), cnt(0), size(5) {}
    ~Queue() {
        delete[] buffer;
    }
    void push(cref data) {
        if (full()) {
            reallocate();
        }
        buffer[(start + cnt) % size] = data;
        ++cnt;
    }
    void pop() {
        if (empty()) {
            throw 0;
        }
        buffer[start].~T();
        start = (start + 1) % size;
        --cnt;
        // 7 8 X 3 4 5 6 c = 6
        //       s
    }
    ref front() {
        if (empty()) {
            throw 0;
        }
        return buffer[start];
    }
    ref rear() {
        if (empty()) {
            throw 0;
        }
        return buffer[(start + cnt - 1) % size];
    }
    cref front() const {
        if (empty()) {
            throw 0;
        }
        return buffer[start];
    }
    cref rear() const {
        if (empty()) {
            throw 0;
        }
        return buffer[(start + cnt - 1) % size];
    }
    bool full() const {
        return count() == size;
    }
    bool empty() const {
        return count() == 0;
    }
    int count() const {
        return cnt;
    }
    void reallocate() {
        auto newBuffer = new T[size * factor];
        for (int i = 0; i < cnt; ++i) {
            newBuffer[i] = buffer[(start + i) % size];
        }
        delete[] buffer;
        buffer = newBuffer;
        start = 0;
        size *= factor;
        // dump();
    }
    void print() const {
        for (int i = 0; i < cnt; ++i) {
            std::cout << std::setw(3) << buffer[(start + i) % size] << ' ';
        }
        std::cout << '\n';
    }
    void dump() const {
        for (int i = 0; i < size; ++i) {
            std::cout << std::setw(3) << buffer[i] << ' ';
        }
        std::cout << '\n';
        for (int i = 0; i < size; ++i) {
            auto c = ' ';
            if (start == i) {
                c = 'S';
            } else if ((start + cnt - 1) % size == i) {
                c = 'C';
            }
            std::cout << std::setw(3) << c << ' ';
        }
        std::cout << '\n' << '\n';
    }
};

#endif // __QUEUE_HPP__
