#include "Record.hpp"

// 2 zdarzenia - ich prawdopodobieństwa oraz prawdopodobieństwo sumy tych zdarzeń
// Uporządkowanie wg klucza

Record::Record() : key(0), A(0.0), B(0.0), Sum(0.0) {}

Record::Record(int key, double a, double b, double s) : key(key), A(a), B(b), Sum(s) {}

void Record::print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << key << " -> ";
    std::cout << "(" << A << ", " << B << ", " << Sum << ")" << std::endl;
}

void Record::writeToFile(std::ofstream& file) const {
    file.write(reinterpret_cast<const char*>(&key), sizeof(key));
    file.write(reinterpret_cast<const char*>(&A), sizeof(A));
    file.write(reinterpret_cast<const char*>(&B), sizeof(B));
    file.write(reinterpret_cast<const char*>(&Sum), sizeof(Sum));
}

void Record::readFromFile(std::ifstream& file) {
    file.read(reinterpret_cast<char*>(&key), sizeof(key));
    file.read(reinterpret_cast<char*>(&A), sizeof(A));
    file.read(reinterpret_cast<char*>(&B), sizeof(B));
    file.read(reinterpret_cast<char*>(&Sum), sizeof(Sum));
}

void Record::randomize() {
    key = rand();
    // Probability of event A and B
    A = (rand() % 100) / 100.0;
    B = (rand() % 100) / 100.0;
    // Sum must be no less than Max(A, B) and no more than Min(1, (A + B))
    double min = std::max(A, B);
    double max = std::min(1.0, A + B);
    Sum = std::round((min + (rand() / (float)RAND_MAX) * (max - min)) * 100) / 100.0;
}

void Record::readFromConsole() {
    std::cout << "Enter key: ";
    std::cin >> key;
    std::cout << "Enter A: ";
    std::cin >> A;
    std::cout << "Enter B: ";
    std::cin >> B;
    std::cout << "Enter Sum: ";
    std::cin >> Sum;
}
