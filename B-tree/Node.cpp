#include "Node.hpp"

Node::Node() : offset(-1), parent(nullptr) {
    elements = std::vector<Element>();
    children = std::vector<int>();
}

Node::Node(int offset) : offset(offset), parent(nullptr) {
    elements = std::vector<Element>();
    children = std::vector<int>();
}

Node::~Node() {
    std::cout << "Node " << offset << " deleted" << std::endl;
}

void Node::writeToFile() const {
    std::fstream file(PAGES_FILE, std::ios::binary | std::ios::in | std::ios::out);
    file.seekp(offset);
    for (int i = 0; i < MAX_ELEMENTS; i++) {
        if (i < elements.size()) {
            elements[i].writeToFile(file);
        }
        else {
            Element element;
            element.writeToFile(file);
        }
    }
    for (int i = 0; i < MAX_ELEMENTS + 1; i++) {
        if (i < children.size()) {
            file.write(reinterpret_cast<const char*>(&children[i]), sizeof(children[i]));
        }
        else {
            int child = -1;
            file.write(reinterpret_cast<const char*>(&child), sizeof(child));
        }
    }
}

void Node::readFromFile() {
    std::fstream file(PAGES_FILE, std::ios::binary | std::ios::in | std::ios::out);
    file.seekg(offset);
    for (int i = 0; i < MAX_ELEMENTS; i++) {
        Element element;
        element.readFromFile(file);
        if (!element.isNull()) {
            elements.push_back(element);
        }
    }
    for (int i = 0; i < MAX_ELEMENTS + 1; i++) {
        int child;
        file.read(reinterpret_cast<char*>(&child), sizeof(child));
        if (child != -1) {
            children.push_back(child);
        }
    }
}

void Node::print() const {
    std::cout << "[";
    for (int i = 0; i < elements.size(); i++) {
        std::cout << elements[i].key;
        if (i < elements.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]";
}

bool Node::isLeaf() const {
    return children.empty();
}

bool Node::isFull() const {
    return elements.size() == MAX_ELEMENTS;
}

bool Node::isOverflown() const {
    return elements.size() > MAX_ELEMENTS;
}

bool Node::isUnderflown() const {
    return elements.size() < MIN_ELEMENTS;
}
