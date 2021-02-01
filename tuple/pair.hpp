#ifndef __PAIR_HPP__
#define __PAIR_HPP__

template <typename T, typename U>
struct Pair {
    T first;
    U second;
    Pair() = default;
    Pair(T const &first, U const &second) : first(first), second(second) {}
    bool operator<(Pair<T, U> const &other) const {
        return first < other.first ||
               first == other.first && second < other.second;
    }
    ~Pair() = default;
};

#endif // __PAIR_HPP__
