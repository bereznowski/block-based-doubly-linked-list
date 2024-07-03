#pragma once

#include <cstdio>
#include "Node.h"

class Iterator
{
public:
    Node* nodePointer;
    unsigned short int positionPointer;

    Iterator();
    Iterator(Node* nodePointer, unsigned short int positionPointer);
    ~Iterator();
};
