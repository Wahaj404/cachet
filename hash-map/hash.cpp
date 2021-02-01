#include "hash.hpp"

#include <climits>
#include <cmath>
#include <limits>

constexpr int primes[] = {
    7,       13,      29,      43,      73,      107,     127,     131,
    137,     139,     149,     151,     157,     163,     167,     173,
    179,     181,     191,     193,     197,     199,     211,     223,
    227,     229,     233,     239,     241,     251,     257,     263,
    269,     271,     277,     281,     283,     293,     307,     311,
    313,     317,     331,     337,     347,     349,     353,     431,
    521,     631,     761,     919,     1103,    1327,    1597,    1931,
    2333,    2801,    3371,    4049,    4861,    5839,    7013,    8419,
    10103,   12143,   14591,   17519,   21023,   25229,   30293,   36353,
    43627,   52361,   62851,   75431,   90523,   108631,  130363,  156437,
    187751,  225307,  270371,  324449,  389357,  467237,  560689,  672827,
    807403,  968897,  1162687, 1395263, 1674319, 2009191, 2411033, 2893249,
    3471899, 4166287, 4999559, 5999471, 7199369, 9270769, 14000839};

constexpr auto A = 54059u, B = 76963u, C = 86969u;

unsigned hash(const std::string &key) {
    auto h = 31u;
    for (char i : key) {
        h = (h * A) ^ (i * B);
    }
    return h;
}

unsigned hash(int key) {
    return hash(std::to_string(key));
}

template <class T, unsigned N>
static constexpr int length(const T (&arr)[N]) {
    return N;
}

static bool isPrime(int num) {
    if (num & 1 != 0) {
        auto limit = static_cast<int>(std::sqrt(num));
        for (int div = 3; div <= limit; div += 2) {
            if (num % div == 0) {
                return false;
            }
        }
        return true;
    }
    return num == 2;
}

int nextPrime(int from) {
    if (from > primes[length(primes) - 1]) {
        for (int i = (from | 1); i < std::numeric_limits<int>::max(); i += 2) {
            if (isPrime(i) && ((i - 1) % 101 != 0))
                return i;
        }
        return from;
    }

    for (int i = 0; i < length(primes); ++i) {
        if (primes[i] > from) {
            return primes[i];
        }
    }

    return -1;
}