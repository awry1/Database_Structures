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

    void print() const;
    void writeToFile(std::ofstream& file) const;
    void readFromFile(std::ifstream& file);

    void randomize();
    void readFromConsole();
};
