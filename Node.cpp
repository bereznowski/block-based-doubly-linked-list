#include "Node.h"

Node::Node(unsigned short int maxNumberOfKeys)
{
    next = nullptr;
    prev = nullptr;
    numberOfKeys = 0;
    keys = new unsigned long long [maxNumberOfKeys];
}

Node::Node(unsigned long long key, unsigned short int maxNumberOfKeys)
{
    next = nullptr;
    prev = nullptr;
    numberOfKeys = 1;
    keys = new unsigned long long [maxNumberOfKeys];
    keys[0] = key;
}

Node::~Node()
{
    next = nullptr;
    prev = nullptr;
    numberOfKeys = 0;
    delete[] keys;
    keys = nullptr;
}