#pragma once
#include <vector>

class Fibonacci {
public:
    std::vector<int> fib;

    Fibonacci();

    void reset();
    void restore(int n);
    int next();
    int next(int n);
    int closestFib(int n);
    bool isFib(int n);
};
