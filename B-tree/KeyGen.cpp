#include "KeyGen.hpp"

KeyGen::KeyGen(int n, bool random) : initialSize(n), succeedingKey(n), random(random) {
    for (int i = n - 1; i >= 0; i--) {
        keys.push_back(i);
    }
    if (random) {
        std::random_shuffle(keys.begin(), keys.end());
    }
}

int KeyGen::nextKey(){
    if (keys.empty()) {
        addKeys();
    }
    int key = keys.back();
    keys.pop_back();
    return key;
}

void KeyGen::addKeys() {
    for (int i = initialSize - 1; i >= 0; i--) {
        keys.push_back(i + succeedingKey);
    }
    if (random) {
        std::random_shuffle(keys.begin(), keys.end());
    }
    succeedingKey += initialSize;
}
