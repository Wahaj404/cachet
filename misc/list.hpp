#ifndef __LIST_HPP__
#define __LIST_HPP__

#include <iostream>

template <typename T>
class List {
    using ref = T &;
    using cref = const T &;
    struct Node {
        T data;
        Node *prev;
        Node *next;
        Node(cref data, Node *prev = nullptr, Node *next = nullptr) :
            data(data), prev(prev), next(next) {}
    };

private:
    Node *head;
    Node *tail;
    int _size;

public:
    List() : head(nullptr), tail(nullptr), _size(0) {}
    void pushFront(cref data) {
        if (empty()) {
            head = tail = new Node(data);
        } else {
            head = (head->prev = new Node(data, nullptr, head));
        }
        ++_size;
    }
    void pushBack(cref data) {
        if (empty()) {
            head = tail = new Node(data);
        } else {
            tail = (tail->next = new Node(data, tail, nullptr));
        }
        ++_size;
    }
    int size() const {
        return _size;
    }
    bool empty() const {
        return _size == 0;
    }
    ~List() {}
    void print() const {
        for (auto cur = head; cur != nullptr; cur = cur->next) {
            std::cout << cur->data << ' ';
        }
        std::cout << '\n';
    }
};

#endif // __LIST_HPP__
