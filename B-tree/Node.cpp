#include "Node.hpp"

Node::Node() : offset(-1) {
    for (int i = 0; i < MAX_ELEMENTS; i++) {
        elements[i] = nullptr;
        children[i] = -1;
    }
    children[MAX_ELEMENTS] = -1;
}

Node::Node(int offset) : offset(offset) {
    std::fstream file(PAGES_FILE, std::ios::binary | std::ios::in | std::ios::out);
    file.seekg(offset);
    for (int i = 0; i < MAX_ELEMENTS; i++) {
        elements[i] = new Element();
        elements[i]->readFromFile(file);
    }
    for (int i = 0; i < MAX_ELEMENTS + 1; i++) {
        file.read(reinterpret_cast<char*>(&children[i]), sizeof(children[i]));
    }
}