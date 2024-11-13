#include "Block.h"

void runFirstLoop(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);

    for (int i = 0; i < 3; ++i) {
        Record r1;
        r1.randomize();
        Record r2;
        r2.randomize();
        Record r3;
        r3.randomize();
        Record r4;
        r4.randomize();
        Block block({ r1, r2, r3, r4 });
        block.print();
        block.writeToFile(outFile);
    }
    // One extra record
    Record r5(10, 10, 10);
    r5.print();
    r5.writeToFile(outFile);

    outFile.close();
}

void runSecondLoop(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);

    // More blocks than in the first loop
    for (int i = 0; i < 4; ++i) {
        Block block;
        block.readFromFile(inFile);
        block.print();
    }

    inFile.close();
}

int main() {
    srand(time(NULL));

    const std::string filename = "records.bin";

    runFirstLoop(filename);

    std::cout << "\n\n";

    runSecondLoop(filename);

    return 0;
}
