#ifndef __HASH_HPP__
#define __HASH_HPP__

#include <string>

unsigned hash(std::string const &key);

unsigned hash(int key);

int nextPrime(int prev);

#endif // __HASH_HPP__
