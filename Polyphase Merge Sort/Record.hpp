#pragma once
#include <fstream>
#include <iomanip>
#include <iostream>

class Record {
public:
    double A;
    double B;
    double Sum;
    double Product;

    Record();
    Record(double a, double b, double s);

    void print() const;
    void writeToFile(std::ofstream& file) const;
    void readFromFile(std::ifstream& file);

    void product();
    void randomize();
    void readFromConsole();
};
