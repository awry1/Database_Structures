#pragma once
#include <vector>

class KeyGen {
public:
    std::vector<int> keys;
    int initialSize;
    int succeedingKey;

    KeyGen(int n, bool random);

    int nextKey();
    void addKeys();
};
