#include "KeyGen.hpp"

KeyGen::KeyGen() {
    for (int i = 0; i < INT_MAX; i++) {
        keys.push_back(i);
    }
    for (int i = 0; i < INT_MAX; i++) {
        int j = rand() % INT_MAX;
        int temp = keys[i];
        keys[i] = keys[j];
        keys[j] = temp;
    }
    initialSize = INT_MAX;
    succeedingKey = INT_MAX;
}

KeyGen::KeyGen(int n) {
    for (int i = 0; i < n; i++) {
        keys.push_back(i);
    }
    for (int i = 0; i < n; i++) {
        int j = rand() % n;
        int temp = keys[i];
        keys[i] = keys[j];
        keys[j] = temp;
    }
    initialSize = n;
    succeedingKey = n;
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
    for (int i = 0; i < initialSize; i++) {
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
