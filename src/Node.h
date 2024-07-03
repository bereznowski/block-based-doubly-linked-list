#pragma once

class Node
{
public:
    Node* next;
    Node* prev;
    unsigned short int numberOfKeys;
    unsigned long long* keys;

    Node(unsigned short int maxNumberOfKeys);
    Node(unsigned long long key, unsigned short int maxNumberOfKeys);
    ~Node();
};