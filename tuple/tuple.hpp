#ifndef __TUPLE_HPP__
#define __TUPLE_HPP__

#include <iostream>

#define show std::cerr << __PRETTY_FUNCTION__ << '\n'

template <unsigned int N, typename T, typename... Ts>
struct TupleImpl {
    T data;
    TupleImpl<N + 1, Ts...> tup;
    TupleImpl() = default;
    TupleImpl(T const &data, Ts... ts) : data(data), tup(ts...) {}
};

template <unsigned int N, typename T>
struct TupleImpl<N, T> {
    T data;
    TupleImpl() = default;
    TupleImpl(T const &data) : data(data) {}
};

template <unsigned int I, unsigned int N, typename... Ts>
auto &get(TupleImpl<N, Ts...> &tup) {
    if constexpr (I == N) {
        return tup.data;
    } else {
        return get<I>(tup.tup);
    }
}

template <typename... Ts>
using Tuple = TupleImpl<0, Ts...>;

template <typename... Ts>
auto makeTuple(Ts... args) {
    return Tuple<Ts...>(args...);
}

#endif // __TUPLE_HPP__
