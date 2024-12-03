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
}

int KeyGen::nextKey(){
    if (keys.empty()) {
        return -1;
    }
    int key = keys.back();
    keys.pop_back();
    return key;
}
