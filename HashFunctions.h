#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

#include <string>

unsigned int SDBMHash(std::string str, unsigned int num_buckets) {
    unsigned int hash = 0;
    unsigned int len = str.length();

    for (unsigned int i = 0; i < len; i++) {
        hash = ((str[i]) + (hash << 6) + (hash << 16) - hash) % num_buckets;
    }

    return hash;
}

unsigned int simpleHash(std::string s, unsigned int numBucket) {
    const int p = 31;
    const int m = 1e9 + 9;
    unsigned int hash_value = 0;
    long long p_pow = 1;
    for (char c : s) {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value % numBucket;
}

unsigned int djb2(std::string str, unsigned int numBucket) {
    unsigned int hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % numBucket;
}

// Function pointer for hash function (default set in main.cpp)
extern unsigned int (*hash_func)(std::string, unsigned int);

#endif // HASH_FUNCTIONS_H