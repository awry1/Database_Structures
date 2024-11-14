#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

class Record {
    public:
        double A;
        double B;
        double Sum;
        double Product;

        Record() = default;
        Record(double a, double b, double s);

        Record compare(Record other) const;
        void randomize();
        void print() const;
        void writeToFile(std::ofstream &file) const;
        void readFromFile(std::ifstream &file);
        double product() const;
};
