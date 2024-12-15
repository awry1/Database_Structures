#include "Btree.hpp"

Btree::Btree() {
    loadedNodes = std::vector<Node*>();
    freeNodes = std::vector<int>();
    freeRecords = std::vector<int>();
    root = new Node(0);

    // Chceck if 2 neccessary files exist, if not, create them
    std::fstream file1(PAGES_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file1.is_open()) {
        file1.open(PAGES_FILE, std::ios::binary | std::ios::out);
        root->writeToFile();
        file1.close();
    }
    std::fstream file2(RECORDS_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file2.is_open()) {
        file2.open(RECORDS_FILE, std::ios::binary | std::ios::out);
        file2.close();
    }

    root->readFromFile();
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

void Btree::printRecord() {
    std::cout << "Find record" << std::endl;
    int key;
    std::cout << "Enter key: ";
    std::cin >> key;
    std::cin.ignore();
    printRecord(key);
}

void Btree::printTree() {
    std::cout << "Printing tree" << std::endl;
    print(root);
    //printReverse(root);
    std::cout << std::endl;
}

void Btree::printAllRecords() {
    std::cout << "Printing recordss in order" << std::endl;
    
    printALL(root);
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
    if (node->isOverflown()) {
        fixup(node);
    }
    else {
        node->writeToFile();
    }
    unloadNodes();
    std::cout << "key " << element.key << " added" << std::endl;
}

void Btree::insertRecord(Record record) {
    Element element = findElement(record.key);
    if (!element.isNull()) {
        std::cout << "key " << record.key << " already exists" << std::endl;
    }
    else {
        int offset = -1;
        if (freeRecords.empty()) {
            std::fstream file(RECORDS_FILE, std::ios::binary | std::ios::in | std::ios::out);
            file.seekg(0, std::ios::end);
            offset = file.tellg();
            file.close();
        }
        else {
            offset = freeRecords.back();
            freeRecords.pop_back();
        }
        element.key = record.key;
        element.offset = offset;
        element.update(record);
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
        std::cout << "key " << key << " not found" << std::endl;
    }
}

void Btree::fixup(Node* node) {
    Node* parent = node->parent;
    if (parent != nullptr) {
        Node* leftSibling = nullptr;
        Node* rightSibling = nullptr;
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
        if (node->isOverflown()) {
            fixOverflow(node, leftSibling, rightSibling, i);
        }
        else if (node->isUnderflown()) {
            fixUnderflow(node, leftSibling, rightSibling, i);
        }
    }
    else {
        if (node->isOverflown()) {
            split(node);
        }
    }
}

void Btree::fixOverflow(Node* node, Node* left, Node* right, int index) {
    if (left != nullptr && left->canReceive()) {
        right = node;
        index--;
    }
    else if (right != nullptr && right->canReceive()) {
        left = node;
    }
    else {
        split(node);
        return;
    }

    Node* parent = node->parent;

    std::vector <Element> elements;
    std::vector<int> children;
    elements.insert(elements.end(), left->elements.begin(), left->elements.end());
    elements.insert(elements.end(), parent->elements[index]);
    elements.insert(elements.end(), right->elements.begin(), right->elements.end());
    children.insert(children.end(), left->children.begin(), left->children.end());
    children.insert(children.end(), right->children.begin(), right->children.end());

    int mid = elements.size() / 2;
    parent->elements[index] = elements[mid];
    left->elements = std::vector<Element>(elements.begin(), elements.begin() + mid);
    right->elements = std::vector<Element>(elements.begin() + mid + 1, elements.end());
    if (!left->isLeaf()) {
        left->children = std::vector<int>(children.begin(), children.begin() + mid + 1);
        right->children = std::vector<int>(children.begin() + mid + 1, children.end());
    }

    left->writeToFile();
    right->writeToFile();
    parent->writeToFile();
}

void Btree::fixUnderflow(Node* node, Node* left, Node* right, int index) {
}

void Btree::split(Node* node) {
    int offset = -1;
    if (freeNodes.empty()) {
        std::fstream file(PAGES_FILE, std::ios::binary | std::ios::in | std::ios::out);
        file.seekg(0, std::ios::end);
        offset = file.tellg();
        file.close();
    }
    else {
        offset = freeNodes.back();
        freeNodes.pop_back();
    }
    Node* sibling = new Node(offset);

    std::vector<Element> elements = node->elements;
    std::vector<int> children = node->children;

    int mid = elements.size() / 2;
    Element middle = elements[mid];
    node->elements = std::vector<Element>(elements.begin(), elements.begin() + mid);
    sibling->elements = std::vector<Element>(elements.begin() + mid + 1, elements.end());
    if (!node->isLeaf()) {
        node->children = std::vector<int>(children.begin(), children.begin() + mid + 1);
        sibling->children = std::vector<int>(children.begin() + mid + 1, children.end());
    }
    node->writeToFile();
    sibling->writeToFile();

    Node* parent = node->parent;
    if (parent != nullptr) {
        int i = 0;
        while (i < parent->children.size() && parent->children[i] != node->offset) {
            i++;
        }
        parent->elements.insert(parent->elements.begin() + i, middle);
        parent->children.insert(parent->children.begin() + i + 1, sibling->offset);

        if (parent->isOverflown()) {
            fixup(parent);
        }
        else {
            parent->writeToFile();
        }
    }
    else {
        offset = -1;
        if (freeNodes.empty()) {
            std::fstream file(PAGES_FILE, std::ios::binary | std::ios::in | std::ios::out);
            file.seekg(0, std::ios::end);
            offset = file.tellg();
            file.close();
        }
        else {
            offset = freeNodes.back();
            freeNodes.pop_back();
        }
        root->offset = offset;
        root->writeToFile();

        parent = new Node(0);
        parent->elements.push_back(middle);
        parent->children.push_back(node->offset);
        parent->children.push_back(sibling->offset);
        parent->writeToFile();
        root = parent;
    }
}

void Btree::printRecord(int key) {
    Element element = findElement(key);
    if (!element.isNull()) {
        element.print();
    }
    else {
        std::cout << "key " << key << " not found" << std::endl;
    }
}

void Btree::print(Node* node, const std::string& prefix, bool isLast) {
    if (node != root) {
        std::cout << prefix << (isLast ? "└──" : "├──");
    }
    node->print();
    std::cout << std::endl;
    std::string newPrefix = prefix;
    if (node != root) {
        newPrefix += (isLast ? "    " : "│   ");
    }
    for (int i = 0; i < node->children.size(); i++) {
        Node* child = new Node(node->children[i]);
        loadedNodes.push_back(child);
        child->readFromFile();
        child->parent = node;
        print(child, newPrefix, i == node->children.size() - 1);
    }
}

void Btree::printReverse(Node* node, const std::string& prefix, bool isLast) {
    if (node != root) {
        std::cout << prefix << (isLast ? "└──" : "├──");
    }
    node->print();
    std::cout << std::endl;
    std::string newPrefix = prefix;
    if (node != root) {
        newPrefix += (isLast ? "    " : "│   ");
    }
    for (int i = node->children.size() - 1; i >= 0; i--) {
        Node* child = new Node(node->children[i]);
        loadedNodes.push_back(child);
        child->readFromFile();
        child->parent = node;
        printReverse(child, newPrefix, i == 0);
    }
}

void Btree::printALL(Node* node) {
    if (node->isLeaf()) {
        for (Element element : node->elements) {
            element.print();
        }
    }
    else {
        for (int i = 0; i < node->elements.size(); i++) {
            Node* child = new Node(node->children[i]);
            loadedNodes.push_back(child);
            child->readFromFile();
            child->parent = node;
            printALL(child);
            Element element = node->elements[i];
            element.print();
        }
        Node* lastChild = new Node(node->children[node->children.size() - 1]);
        loadedNodes.push_back(lastChild);
        lastChild->readFromFile();
        lastChild->parent = node;
        printALL(lastChild);
    }
}

void Btree::clearFiles() {
    std::cout << "Tree emptied" << std::endl;
    std::fstream file1(PAGES_FILE, std::ios::binary | std::ios::out);
    root->elements.clear();
    root->children.clear();
    root->writeToFile();
    file1.close();
    std::fstream file2(RECORDS_FILE, std::ios::binary | std::ios::out);
    file2.close();
}

void Btree::unloadNodes() {
    while (!loadedNodes.empty()) {
        delete(loadedNodes.back());
        loadedNodes.pop_back();
    }
}

void Btree::deleteTree() {
    std::cout << "Freeing memory" << std::endl;
    unloadNodes();
    delete(root);
}
