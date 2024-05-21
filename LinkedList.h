#pragma once

#include "Node.h"
#include "Iterator.h"
#include "Direction.h"

#define numberOfListIterators 12 // 10 numbered iterators (0-9), BEG, and END
#define BEG 10
#define END 11


class LinkedList
{
private:
    Iterator listOfIterators[numberOfListIterators];
    unsigned short int blockSize;

public:
    LinkedList()
    {
        blockSize = 1;
        for(unsigned short int i = 0; i < numberOfListIterators; i++)
        {
            listOfIterators[i].nodePointer = nullptr;
            listOfIterators[i].positionPointer = 0;
        }
    }

    LinkedList(unsigned short int blockSize)
    {
        this->blockSize = blockSize;
        for(unsigned short int i = 0; i < numberOfListIterators; i++)
        {
            listOfIterators[i].nodePointer = nullptr;
            listOfIterators[i].positionPointer = 0;
        }
    }

    ~LinkedList()
    {
    }

    void addFirstNode(unsigned long long key);
    void shiftIteratorsInNode(Node* node, unsigned short int oldPosition, unsigned short int newPosition);
    void shiftKeysInNode(Node* node, Direction direction, unsigned short int startingPosition);
    void shiftIteratorsToNewNode(Node* oldNode, unsigned short int oldNodePosition, Node* newNode, unsigned short int newNodePosition);
    void splitNode(Node* firstNode);
    void mergeNodes(Node* firstNode, Node* secondNode);
    void mergePairOfAlmostEmptyNodes(Node* firstNode, Node* secondNode);
    void addKeyBefore(unsigned short int iteratorNumber, unsigned long long key);
    void addKeyAfter(unsigned short int iteratorNumber, unsigned long long key);
    void initializeIterator(unsigned short int newIteratorNumber, unsigned short int oldIteratorNumber);
    void moveIterator(unsigned short int iteratorNumber, Direction direction);
    void removeKey(unsigned short int iteratorNumber);
    void printList() const;
    void printList(unsigned short int iteratorNumber) const;
};
