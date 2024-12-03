#pragma once
#include "Element.hpp"
constexpr auto PAGE_SIZE = 2;
constexpr auto MIN_ELEMENTS = PAGE_SIZE;
constexpr auto MAX_ELEMENTS = 2 * PAGE_SIZE;

class Node {
public:
    Element* elements[MAX_ELEMENTS] = { nullptr, nullptr, nullptr, nullptr }; // elements[i] is the i-th element in the node
    int children[MAX_ELEMENTS + 1] = { -1, -1, -1, -1, -1 }; // children[i] is the offset of the i-th child in the file

    Node();
};
