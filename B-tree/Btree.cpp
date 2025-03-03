#include "Btree.hpp"

Btree::Btree(bool display) : display(display), reads(0), writes(0) {
    loadedNodes = std::vector<Node*>();
    freeNodes = std::vector<int>();
    freeRecords = std::vector<int>();
    root = new Node(0);

    // Chceck if neccessary files exist, if not, create them
    std::fstream file1(PAGES_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (file1.is_open()) {
        root->readFromFile();
        if (root->isEmpty()) {
            root->offset = 0;
            root->children.clear();
            root->writeToFile();
        }
        file1.close();
    }
    else {
        file1.open(PAGES_FILE, std::ios::binary | std::ios::out);
        file1.close();
    }
    std::fstream file2(RECORDS_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file2.is_open()) {
        file2.open(RECORDS_FILE, std::ios::binary | std::ios::out);
        file2.close();
    }

    // Load free nodes and records from files
    int offset;
    std::ifstream file3(FREE_NODES_FILE);
    if (file3.is_open()) {
        while (file3 >> offset) {
            freeNodes.push_back(offset);
        }
        file3.close();
    }
    std::ifstream file4(FREE_RECORDS_FILE);
    if (file4.is_open()) {
        while (file4 >> offset) {
            freeRecords.push_back(offset);
        }
        file4.close();
    }
}

void Btree::insertRecord() {
    std::cout << "Insert record" << std::endl;
    Record record;
    record.readFromConsole();
    insertRecord(record);
    if (display) {
        printTree();
    }
    std::cout << "Reads: " << reads << " Writes: " << writes << std::endl;
    reads = 0;
    writes = 0;
}

void Btree::updateRecord() {
    std::cout << "Update record" << std::endl;
    Record record;
    record.readFromConsole();
    updateRecord(record);
    std::cout << "Reads: " << reads << " Writes: " << writes << std::endl;
    reads = 0;
    writes = 0;
}

void Btree::deleteRecord() {
    std::cout << "Delete record" << std::endl;
    int key;
    std::cout << "Enter key: ";
    std::cin >> key;
    std::cin.ignore();
    deleteRecord(key);
    if (display) {
        printTree();
    }
    std::cout << "Reads: " << reads << " Writes: " << writes << std::endl;
    reads = 0;
    writes = 0;
}

void Btree::printRecord() {
    std::cout << "Find record" << std::endl;
    int key;
    std::cout << "Enter key: ";
    std::cin >> key;
    std::cin.ignore();
    printRecord(key);
    std::cout << "Reads: " << reads << " Writes: " << writes << std::endl;
    reads = 0;
    writes = 0;
}

void Btree::printTree() {
    print(root);
    //printReverse(root);
    std::cout << std::endl;
}

void Btree::printAllRecords() {
    printALL(root);
    std::cout << std::endl;
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
        child->readFromFile(); reads += 1;
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
        node->writeToFile(); writes += 1;
    }
    unloadNodes();
    if (display) {
        std::cout << "key " << element.key << " added" << std::endl;
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

void Btree::deleteElement(Element element) {
    Node* node = element.parent;
    int i = 0;
    while (i < node->elements.size() && element.key != node->elements[i].key) {
        i++;
    }
    if (!node->isLeaf()) {
        Node* child = new Node(node->children[i]);
        loadedNodes.push_back(child);
        child->readFromFile(); reads += 1;
        child->parent = node;
        while (!child->isLeaf()) {
            Node* nextChild = new Node(child->children.back());
            loadedNodes.push_back(nextChild);
            nextChild->readFromFile(); reads += 1;
            nextChild->parent = child;
            child = nextChild;
        }
        Element predecessor = child->elements.back();
        child->elements.pop_back();
        node->elements[i] = predecessor;
        node->writeToFile(); writes += 1;
        
        if (child->isUnderflown()) {
            fixup(child);
        }
        else {
            child->writeToFile(); writes += 1;
        }
    }
    else {
        node->elements.erase(node->elements.begin() + i);
        if (node != root) {
            if (node->isUnderflown()) {
                fixup(node);
            }
            else {
                node->writeToFile(); writes += 1;
            }
        }
        else {
            node->writeToFile(); writes += 1;
        }
    }
    unloadNodes();
    if (display) {
        std::cout << "key " << element.key << " deleted" << std::endl;
    }
}

void Btree::deleteRecord(int key) {
    Element element = findElement(key);
    if (!element.isNull()) {
        freeRecords.push_back(element.offset);
        deleteElement(element);
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
            leftSibling->readFromFile(); reads += 1;
            leftSibling->parent = parent;
        }
        if (i < parent->children.size() - 1) {
            rightSibling = new Node(parent->children[i + 1]);
            loadedNodes.push_back(rightSibling);
            rightSibling->readFromFile(); reads += 1;
            rightSibling->parent = parent;
        }
        if (node->isOverflown()) {
            fixOverflow(node, leftSibling, rightSibling, i);
        }
        else if (node->isUnderflown()) {
            fixUnderflow(node, leftSibling, rightSibling, i);
        }
    }
    else if (node->isOverflown()) {
        split(node);
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

    left->writeToFile(); writes += 1;
    right->writeToFile(); writes += 1;
    parent->writeToFile(); writes += 1;
}

void Btree::fixUnderflow(Node* node, Node* left, Node* right, int index) {
    if (left != nullptr && left->canGive()) {
        right = node;
        index--;
    }
    else if (right != nullptr && right->canGive()) {
        left = node;
    }
    else {
        merge(node, left, right, index);
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

    left->writeToFile(); writes += 1;
    right->writeToFile(); writes += 1;
    parent->writeToFile(); writes += 1;
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
    sibling->writeToFile(); writes += 1;

    Node* parent = node->parent;
    if (parent != nullptr) {
        node->writeToFile(); writes += 1;

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
            parent->writeToFile(); writes += 1;
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
        node->offset = offset;
        node->writeToFile(); writes += 1;

        parent = new Node(0);
        parent->elements.push_back(middle);
        parent->children.push_back(node->offset);
        parent->children.push_back(sibling->offset);
        parent->parent = nullptr;
        parent->writeToFile(); writes += 1;
        root = parent;
    }
}

void Btree::merge(Node* node, Node* left, Node* right, int index) {
    Node* parent = node->parent;
    if (index == 0) {
        left = node;
    }
    else {
        right = node;
        index--;
    }

    std::vector <Element> elements;
    std::vector<int> children;
    elements.insert(elements.end(), left->elements.begin(), left->elements.end());
    elements.insert(elements.end(), parent->elements[index]);
    elements.insert(elements.end(), right->elements.begin(), right->elements.end());
    children.insert(children.end(), left->children.begin(), left->children.end());
    children.insert(children.end(), right->children.begin(), right->children.end());

    left->elements = elements;
    if (!left->isLeaf()) {
        left->children = children;
    }
    freeNodes.push_back(right->offset);

    parent->elements.erase(parent->elements.begin() + index);
    parent->children.erase(parent->children.begin() + index + 1);
    if (parent->isEmpty()) {
        freeNodes.push_back(left->offset);
        left->offset = 0;
        left->parent = nullptr;
        for (int i = 0; i < loadedNodes.size(); i++) {
            if (loadedNodes[i] == left) {
                loadedNodes.erase(loadedNodes.begin() + i);
                break;
            }
        }
        root = left;
        root->writeToFile(); writes += 1;
    }
    else {
        left->writeToFile(); writes += 1;
        if (parent->isUnderflown()) {
            fixup(parent);
        }
        else {
            parent->writeToFile(); writes += 1;
        }
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
    std::ofstream file1(PAGES_FILE, std::ios::binary);
    root->elements.clear();
    root->children.clear();
    root->writeToFile();
    file1.close();
    std::ofstream file2(RECORDS_FILE, std::ios::binary);
    file2.close();
    std::ofstream file3(FREE_NODES_FILE);
    file3.close();
    std::ofstream file4(FREE_RECORDS_FILE);
    file4.close();
    freeNodes.clear();
    freeRecords.clear();
    reads = 0;
    writes = 0;
}

void Btree::unloadNodes() {
    while (!loadedNodes.empty()) {
        delete loadedNodes.back();
        loadedNodes.pop_back();
    }
}

void Btree::deleteTree() {
    std::cout << "Freeing memory" << std::endl;
    unloadNodes();
    delete(root);
    // Save free nodes and records to files
    std::ofstream file1(FREE_NODES_FILE);
    for (int offset : freeNodes) {
        file1 << offset << std::endl;
    }
    file1.close();
    std::ofstream file2(FREE_RECORDS_FILE);
    for (int offset : freeRecords) {
        file2 << offset << std::endl;
    }
    file2.close();
}
