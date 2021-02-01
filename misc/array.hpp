#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

template <typename T, unsigned N>
class Array {
    using ref = T &;
    using cref = const T &;

private:
    T data[N];
    bool outOfBounds(int i) const {
        return i < 0 || i >= N;
    }

public:
    Array() = default;
    Array(const Array &other) {
        for (int i = 0; i < N; ++i) {
            data[i] = other.data[i];
        }
    }
    ref operator[](int i) {
        if (outOfBounds(i)) {
            throw 0;
        }
        return data[i];
    }
    cref operator[](int i) const {
        if (outOfBounds(i)) {
            throw 0;
        }
        return data[i];
    }
    int size() const {
        return N;
    }
};

#endif // __ARRAY_HPP__
