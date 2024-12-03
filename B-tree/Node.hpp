#pragma once
#include <fstream>
#include "Element.hpp"
constexpr auto PAGES_FILE = "pages.bin";
constexpr auto PAGE_SIZE = 2;
constexpr auto MIN_ELEMENTS = PAGE_SIZE;
constexpr auto MAX_ELEMENTS = 2 * PAGE_SIZE;

class Node {
public:
    int offset; // Node offset in pages.bin
    Element* elements[MAX_ELEMENTS];
    int children[MAX_ELEMENTS + 1]; // Child offset in pages.bin

    Node();
    Node(int offset);
};
