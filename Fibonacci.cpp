#include "Fibonacci.h"

Fibonacci::Fibonacci() {
    fib = { 0 };
}

void Fibonacci::reset() {
    fib = { 0 };
}

void Fibonacci::restore(int n) {
    // Ensures next() = n
    while (fib.back() > n) {
        fib.pop_back();
    }
    while (fib.back() < n) {
        next();
    }
}

int Fibonacci::next() {
    if (fib.size() > 2) {
        fib.push_back(fib[fib.size() - 1] + fib[fib.size() - 2]);
        return fib[fib.size() - 2];
    }
    else if (fib.size() == 2) {
        fib.push_back(1);
        return fib[1];
    }
    else if (fib.size() == 1) {
        fib.push_back(1);
        return fib[0];
    }
    return -1;
}

int Fibonacci::next(int n) {
    // Returns next() > n
    int savePoint = next();
    restore(n);
    int nextFib = next();
    if (nextFib == n) {
        nextFib = next();
    }
    restore(savePoint);
    return nextFib;
}

int Fibonacci::closestFib(int n) {
    // Returns next() >= n
    int savePoint = next();
    restore(n);
    int nextFib = next();
    restore(savePoint);
    return nextFib;
}

bool Fibonacci::isFib(int n) {
    return closestFib(n) == n;
}
