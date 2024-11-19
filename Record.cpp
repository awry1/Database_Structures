#include "Record.h"

// 2 zdarzenia - ich prawdopodobieństwa oraz prawdopodobieństwo sumy tych zdarzeń
// Uporządkowanie wg prawdopodobieństwa iloczynu tych zdarzeń

Record::Record() : A(0.0), B(0.0), Sum(0.0) {
    product();
}

Record::Record(double a, double b, double s) : A(a), B(b), Sum(s) {
    product();
}

void Record::print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << Product << " -> ";
    std::cout << "A:" << A << " B:" << B << " Sum:" << Sum << std::endl;
    //std::cout << std::endl;
}

void Record::writeToFile(std::ofstream& file) const {
    file.write(reinterpret_cast<const char*>(&A), sizeof(A));
    file.write(reinterpret_cast<const char*>(&B), sizeof(B));
    file.write(reinterpret_cast<const char*>(&Sum), sizeof(Sum));
}

void Record::readFromFile(std::ifstream& file) {
    file.read(reinterpret_cast<char*>(&A), sizeof(A));
    file.read(reinterpret_cast<char*>(&B), sizeof(B));
    file.read(reinterpret_cast<char*>(&Sum), sizeof(Sum));
    product();
}

void Record::product() {
    Product = A + B - Sum;
    if (std::abs(Product) < 1e-10) {
        Product = 0.0;
    }
}

void Record::randomize() {
    // Probability of event A and B
    A = (rand() % 100) / 100.0;
    B = (rand() % 100) / 100.0;
    // Sum must be no less than Max(A, B) and no more than Min(1, (A + B))
    double min = std::max(A, B);
    double max = std::min(1.0, A + B);
    Sum = std::round((min + (rand() / (float)RAND_MAX) * (max - min)) * 100) / 100.0;
    // Calculate Product
    product();
}

void Record::readFromConsole() {
    std::cout << "Enter A: ";
    std::cin >> A;
    std::cout << "Enter B: ";
    std::cin >> B;
    std::cout << "Enter Sum: ";
    std::cin >> Sum;
    product();
}

Record Record::compare(Record other) const {
    // Returns lesser record
    if (Product < other.Product) {
        return *this;
    }
    else {
        return other;
    }
}
