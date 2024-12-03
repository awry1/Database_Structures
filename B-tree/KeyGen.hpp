#pragma once
#include <vector>

class KeyGen {
public:
    std::vector<int> keys;

    KeyGen();
    KeyGen(int n);

    int nextKey();
};
