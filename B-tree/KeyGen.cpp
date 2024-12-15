#include "KeyGen.hpp"

KeyGen::KeyGen(int n, bool random) : initialSize(n), succeedingKey(n) {
    for (int i = n - 1; i >= 0; i--) {
        keys.push_back(i);
    }
    if (random) {
        for (int i = 0; i < n; i++) {
            int j = rand() % n;
            int temp = keys[i];
            keys[i] = keys[j];
            keys[j] = temp;
        }
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
    for (int i = 0; i < initialSize; i++) {
        int j = rand() % initialSize;
        int temp = keys[i];
        keys[i] = keys[j];
        keys[j] = temp;
    }
    succeedingKey += initialSize;
}
