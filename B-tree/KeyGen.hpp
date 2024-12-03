#pragma once
#include <vector>

class KeyGen {
public:
    std::vector<int> keys;
    int initialSize;
    int succeedingKey;

    KeyGen();
    KeyGen(int n);

    int nextKey();
    void addKeys();
};
