#include "Record.hpp"

int main() {
    srand(unsigned int(time(NULL)));

    Record r;
    r.randomize();
    r.print();
}
