#pragma once
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

class Record {
public:
    int key;
    double A;
    double B;
    double Sum;

    Record();
    Record(int k, double a, double b, double s);

    void writeToFile(std::fstream& file) const;
    void readFromFile(std::fstream& file);
    void print() const;

    void randomize();
    void readFromConsole();
};
