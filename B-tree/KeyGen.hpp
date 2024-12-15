#pragma once
#include <algorithm>
#include <vector>

class KeyGen {
public:
    std::vector<int> keys;
    int initialSize;
    int succeedingKey;
    bool random;

    KeyGen(int n, bool random);

    int nextKey();
    void addKeys();
};
