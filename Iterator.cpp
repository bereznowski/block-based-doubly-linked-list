#include "Iterator.h"

Iterator::Iterator()
{
    this->nodePointer = nullptr;
    this->positionPointer = 0;
}

Iterator::Iterator(Node* nodePointer, unsigned short int positionPointer)
{
    this->nodePointer = nodePointer;
    this->positionPointer = positionPointer;
}

Iterator::~Iterator()
{
    nodePointer = nullptr;
    positionPointer = 0;
}
