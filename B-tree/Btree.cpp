#include "Btree.hpp"

Btree::Btree() {
    freeNodes = std::vector<int>();
    freeRecords = std::vector<int>();

    root = Node(0);
}

Node Btree::findNode(int key) {
    Node node = root;
    while (!node.isLeaf()) {
        int i = 0;
        while (i < node.elements.size() && key > node.elements[i].key) {
            i++;
        }
        if (i < node.elements.size() && key == node.elements[i].key) {
            return node;
        }
        Node child(node.children[i]);
        child.readFromFile();
        child.parent = &node;
        node = child;
    }
    return node;
}

Element Btree::findElement(int key) {
    Node node = findNode(key);
    for (Element element : node.elements) {
        if (element.key == key) {
            element.parent = &node;
            return element;
        }
    }
    return Element();
}

void Btree::findRecord() {
    std::cout << "Find record" << std::endl;
    int key;
    std::cout << "Enter key: ";
    std::cin >> key;
    std::cin.ignore();
    findRecord(key);
}

void Btree::findRecord(int key) {
    Element element = findElement(key);
    if (!element.isNull()) {
        element.print();
    }
    else {
        std::cout << "key " << key << " not found" << std::endl;
    }
}

void Btree::insertRecord() {
    std::cout << "Insert record" << std::endl;
    Record record;
    record.readFromConsole();
    insertRecord(record);
}

void Btree::insertRecord(Record record) {
    Element element = findElement(record.key);
    if (!element.isNull()) {
        std::cout << "key " << record.key << " already exists" << std::endl;
    }
    else {
        Node node = *element.parent;
        // insertElement(node, record);
        std::cout << "key " << record.key << " added" << std::endl; // WIP
    }
}

void Btree::updateRecord() {
    std::cout << "Update record" << std::endl;
    Record record;
    record.readFromConsole();
    updateRecord(record);
}

void Btree::updateRecord(Record record) {
    Element element = findElement(record.key);
    if (!element.isNull()) {
        element.update(record);
    }
    else {
        std::cout << "key " << record.key << " not found" << std::endl;
    }
}

void Btree::deleteRecord() {
    std::cout << "Delete record" << std::endl;
    int key;
    std::cout << "Enter key: ";
    std::cin >> key;
    std::cin.ignore();
    deleteRecord(key);
}

void Btree::deleteRecord(int key) {
    Element element = findElement(key);
    if (!element.isNull()) {
        // deleteElement(element)
        std::cout << "key " << key << " deleted" << std::endl; // WIP
    }
    else {
        std::cout << "key " << key<< " not found" << std::endl;
    }
}

void Btree::printBFS() {
    std::cout << "Printing tree (BFS)" << std::endl;
    std::queue<Node> queue;
    queue.push(root);
    Node* parent = nullptr;
    while (!queue.empty()) {
        Node node = queue.front();
        queue.pop();
        if (node.parent != parent) {
            std::cout << std::endl;
            parent = node.parent;
            std::cout << "Parent: ";
            parent->print();
        }
        node.print();
        for (int child : node.children) {
            Node childNode(child);
            childNode.readFromFile();
            childNode.parent = &node;
            queue.push(childNode);
        }
    }
}

void Btree::printInOrder() {
    std::cout << "Printing tree (in order)" << std::endl;
}
