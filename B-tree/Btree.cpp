#include "Btree.hpp"

Btree::Btree() {
    loadedNodes = std::vector<Node*>();
    freeNodes = std::vector<int>();
    freeRecords = std::vector<int>();
    records = 0;

    root = new Node(0);
}

void Btree::findRecord() {
    std::cout << "Find record" << std::endl;
    int key;
    std::cout << "Enter key: ";
    std::cin >> key;
    std::cin.ignore();
    findRecord(key);
}

void Btree::insertRecord() {
    std::cout << "Insert record" << std::endl;
    Record record;
    record.readFromConsole();
    insertRecord(record);
}

void Btree::updateRecord() {
    std::cout << "Update record" << std::endl;
    Record record;
    record.readFromConsole();
    updateRecord(record);
}

void Btree::deleteRecord() {
    std::cout << "Delete record" << std::endl;
    int key;
    std::cout << "Enter key: ";
    std::cin >> key;
    std::cin.ignore();
    deleteRecord(key);
}

void Btree::printBFS() {
    std::cout << "Printing tree (BFS)" << std::endl;
    std::queue<Node*> queue;
    queue.push(root);
    Node* parent = nullptr;
    while (!queue.empty()) {
        Node* node = queue.front();
        queue.pop();
        if (node->parent != parent) {
            std::cout << std::endl;
            parent = node->parent;
            std::cout << "Parent: ";
            parent->print();
        }
        node->print();
        for (int child : node->children) {
            Node* childNode = new Node(child);
            loadedNodes.push_back(childNode);
            childNode->readFromFile();
            childNode->parent = node;
            queue.push(childNode);
        }
    }
    std::cout << std::endl;
    unloadNodes();
}

void Btree::printInOrder() {
    std::cout << "Printing tree (in order)" << std::endl;
}

void Btree::printTree() {
    std::cout << "Printing tree" << std::endl;
    print(root); // Will check later if it works
    std::cout << std::endl;
}

void Btree::print(Node* node, const std::string& prefix, bool isLast) {
    std::cout << prefix << (isLast ? "└── " : "├── ");
    node->print();
    std::string newPrefix = prefix + (isLast ? "    " : "│   ");
    for (int i = 0; i < node->children.size(); i++) {
        Node* child = new Node(node->children[i]);
        loadedNodes.push_back(child);
        child->readFromFile();
        child->parent = node;
        print(child, newPrefix, i == node->children.size() - 1);
    }
}

Node* Btree::findNode(int key) {
    Node* node = root;
    while (!node->isLeaf()) {
        int i = 0;
        while (i < node->elements.size() && key > node->elements[i].key) {
            i++;
        }
        if (i < node->elements.size() && key == node->elements[i].key) {
            return node;
        }
        Node* child = new Node(node->children[i]);
        loadedNodes.push_back(child);
        child->readFromFile();
        child->parent = node;
        node = child;
    }
    return node;
}

Element Btree::findElement(int key) {
    Node* node = findNode(key);
    for (Element element : node->elements) {
        if (element.key == key) {
            element.parent = node;
            return element;
        }
    }
    Element element;
    element.parent = node;
    return element;
}

void Btree::insertElement(Element element) {
    Node* node = element.parent;
    int i = 0;
    while (i < node->elements.size() && element.key > node->elements[i].key) {
        i++;
    }
    node->elements.insert(node->elements.begin() + i, element);
    if (!node->isOverflown()) {
        node->writeToFile();
    }
    else {
        //fixup(node);
        node->writeToFile(); // WIP, temporary
    }
    unloadNodes();
    std::cout << "key " << element.key << " added" << std::endl;
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

void Btree::insertRecord(Record record) {
    Element element = findElement(record.key);
    if (!element.isNull()) {
        std::cout << "key " << record.key << " already exists" << std::endl;
    }
    else {
        int offset = -1;
        if (freeRecords.empty()) {
            offset = records * sizeof(Record);
        }
        else {
            offset = freeRecords.back();
            freeRecords.pop_back();
        }
        element.key = record.key;
        element.offset = offset;
        element.update(record);
        records++;
        insertElement(element);
    }
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

void Btree::fixup(Node* node) {
    Node* parent = node->parent;
    Node* leftSibling = nullptr;
    Node* rightSibling = nullptr;
    if (parent != nullptr) {
        int i = 0;
        while (i < parent->children.size() && parent->children[i] != node->offset) {
            i++;
        }
        if (i > 0) {
            leftSibling = new Node(parent->children[i - 1]);
            loadedNodes.push_back(leftSibling);
            leftSibling->readFromFile();
            leftSibling->parent = parent;
        }
        if (i < parent->children.size() - 1) {
            rightSibling = new Node(parent->children[i + 1]);
            loadedNodes.push_back(rightSibling);
            rightSibling->readFromFile();
            rightSibling->parent = parent;
        }
    }
    if (node->isOverflown()) {
        // Check if node has not full sibling
        // If yes, compensate
        // If no, split
    }
    if (node->isUnderflown()) {
        // Check if node has full sibling
        // If yes, compensate
        // If no, merge
    }
}

void Btree::unloadNodes() {
    while (!loadedNodes.empty()) {
        delete(loadedNodes.back());
        loadedNodes.pop_back();
    }
}

void Btree::deleteTree() {
    unloadNodes();
    delete(root);
}
