#include <stdio.h>
#include "LinkedList.h"

#define firstPosition 0
#define minimalAcceptablePercentage 1.0

using namespace std;

void LinkedList::addFirstNode(unsigned long long key)
{
    Node* newNode = new Node(key, blockSize);

    listOfIterators[BEG].nodePointer = newNode;
    listOfIterators[BEG].positionPointer = firstPosition;
    listOfIterators[END].nodePointer = newNode;
    listOfIterators[END].positionPointer = firstPosition;
    return;
}

void LinkedList::shiftIteratorsInNode(Node* node, unsigned short int oldPosition, unsigned short int newPosition)
{
    for(int i = 0; i < numberOfListIterators; i++)
    {
        if(listOfIterators[i].nodePointer == node && listOfIterators[i].positionPointer == oldPosition)
            listOfIterators[i].positionPointer = newPosition;
    }
}

void LinkedList::shiftKeysInNode(Node* node, Direction direction, unsigned short int startingPosition)
{
    if(direction == Direction::forward)
    {
        for(unsigned short int i = startingPosition; i < node->numberOfKeys - 1; i++)
        {
            node->keys[i] = node->keys[i+1];
            shiftIteratorsInNode(node, i + 1, i);
        }

        node->numberOfKeys--;
        return;
    }

    for(unsigned short int i = node->numberOfKeys; i > startingPosition; i--)
    {
        node->keys[i] = node->keys[i - 1];
        shiftIteratorsInNode(node, i - 1, i);
    }

    node->numberOfKeys++;
}

void LinkedList::shiftIteratorsToNewNode(Node* oldNode, unsigned short int oldNodePosition, Node* newNode, unsigned short int newNodePosition)
{
    for(int i = 0; i < numberOfListIterators; i++)
    {
        if(listOfIterators[i].nodePointer == oldNode && listOfIterators[i].positionPointer == oldNodePosition)
        {
            listOfIterators[i].nodePointer = newNode;
            listOfIterators[i].positionPointer = newNodePosition;
        }
    }
}

void LinkedList::splitNode(Node* firstNode)
{
    Node* secondNode = new Node(blockSize);

    unsigned short int fullNumberOfKeys = firstNode->numberOfKeys;

    firstNode->numberOfKeys = (firstNode->numberOfKeys + 1) / 2;
    secondNode->numberOfKeys = fullNumberOfKeys - firstNode->numberOfKeys;
    
    unsigned short int secondNodePosition = 0;

    for(unsigned short int i = firstNode->numberOfKeys; i < fullNumberOfKeys; i++)
    {
        secondNode->keys[secondNodePosition] = firstNode->keys[i];
        shiftIteratorsToNewNode(firstNode, i, secondNode, secondNodePosition);
        secondNodePosition++;
    }

    secondNode->next = firstNode->next;
    secondNode->prev = firstNode;
    firstNode->next = secondNode;
    
    if(secondNode->next != nullptr)
        secondNode->next->prev = secondNode;
}

void LinkedList::mergeNodes(Node* firstNode, Node* secondNode)
{
    unsigned short int firstNodePosition = firstNode->numberOfKeys;
    unsigned short int secondNodePosition = 0;
    firstNode->numberOfKeys = firstNode->numberOfKeys + secondNode->numberOfKeys;
    
    for(unsigned short int i = firstNodePosition; i < firstNode->numberOfKeys; i++)
    {
        firstNode->keys[i] = secondNode->keys[secondNodePosition];
        shiftIteratorsToNewNode(secondNode, secondNodePosition, firstNode, i);
        secondNodePosition++;
    }

    firstNode->next = secondNode->next;

    if(firstNode->next != nullptr)
        firstNode->next->prev = firstNode;

    delete secondNode;
        
    return;
}

void LinkedList::mergePairOfAlmostEmptyNodes(Node* firstNode, Node* secondNode)
{
    if(firstNode->numberOfKeys + secondNode->numberOfKeys <= minimalAcceptablePercentage * blockSize)
        mergeNodes(firstNode, secondNode);
}

void LinkedList::addKeyBefore(unsigned short int iteratorNumber, unsigned long long key)
{
    // Case 1. Adding first node to the list.
    if(listOfIterators[BEG].nodePointer == nullptr)
    {
        addFirstNode(key);
        return;
    }

    // Case 2. Adding key before BEG.
    if(listOfIterators[iteratorNumber].nodePointer == listOfIterators[BEG].nodePointer && listOfIterators[iteratorNumber].positionPointer == listOfIterators[BEG].positionPointer)
    {
        // Case 2.1. When node is not full.
        if(listOfIterators[iteratorNumber].nodePointer->numberOfKeys < blockSize)
        {
            shiftKeysInNode(listOfIterators[iteratorNumber].nodePointer, Direction::backward, listOfIterators[iteratorNumber].positionPointer);
            listOfIterators[BEG].positionPointer = 0;
            listOfIterators[BEG].nodePointer->keys[listOfIterators[BEG].positionPointer] = key;
            return;
        }

        // Case 2.2. When node is full.
        splitNode(listOfIterators[iteratorNumber].nodePointer);
        shiftKeysInNode(listOfIterators[iteratorNumber].nodePointer, Direction::backward, firstPosition);
        listOfIterators[BEG].positionPointer = 0;
        listOfIterators[BEG].nodePointer->keys[listOfIterators[BEG].positionPointer] = key;
        if(listOfIterators[iteratorNumber].nodePointer->next != nullptr && listOfIterators[iteratorNumber].nodePointer->next->next != nullptr)
            mergePairOfAlmostEmptyNodes(listOfIterators[iteratorNumber].nodePointer->next, listOfIterators[iteratorNumber].nodePointer->next->next);
        return;
    }

    // Case 3. Adding key before other key (!= BEG).
    // Case 3.1. When node is not full.
    if(listOfIterators[iteratorNumber].nodePointer->numberOfKeys < blockSize)
    {
        shiftKeysInNode(listOfIterators[iteratorNumber].nodePointer, Direction::backward, listOfIterators[iteratorNumber].positionPointer);
        listOfIterators[iteratorNumber].nodePointer->keys[listOfIterators[iteratorNumber].positionPointer - 1] = key;
        return;
    }

    // Case 3.2. When node is full.
    splitNode(listOfIterators[iteratorNumber].nodePointer);
    shiftKeysInNode(listOfIterators[iteratorNumber].nodePointer, Direction::backward, listOfIterators[iteratorNumber].positionPointer);
    listOfIterators[BEG].positionPointer = 0;
    listOfIterators[iteratorNumber].nodePointer->keys[listOfIterators[iteratorNumber].positionPointer - 1] = key;

    if(listOfIterators[iteratorNumber].nodePointer->next != nullptr && listOfIterators[iteratorNumber].nodePointer->next->next != nullptr)
        mergePairOfAlmostEmptyNodes(listOfIterators[iteratorNumber].nodePointer->next, listOfIterators[iteratorNumber].nodePointer->next->next);
    if(listOfIterators[iteratorNumber].nodePointer->prev != nullptr && listOfIterators[iteratorNumber].nodePointer->prev->prev != nullptr)
        mergePairOfAlmostEmptyNodes(listOfIterators[iteratorNumber].nodePointer->prev->prev, listOfIterators[iteratorNumber].nodePointer->prev);

    return;
}

void LinkedList::addKeyAfter(unsigned short int iteratorNumber, unsigned long long key)
{
    // Case 1. Adding first node to the list.
    if(listOfIterators[BEG].nodePointer == nullptr)
    {
        addFirstNode(key);
        return;
    }

    // Case 2. Adding key after END.
    if(listOfIterators[iteratorNumber].nodePointer == listOfIterators[END].nodePointer && listOfIterators[iteratorNumber].positionPointer == listOfIterators[END].positionPointer)
    {
        // Case 2.1. When node is not full.
        if(listOfIterators[END].nodePointer->numberOfKeys < blockSize)
        {
            listOfIterators[END].nodePointer->numberOfKeys++;
            listOfIterators[END].positionPointer++;
            listOfIterators[END].nodePointer->keys[listOfIterators[END].positionPointer] = key;
            return;
        }
    
        // Case 2.2. When node is full.
        splitNode(listOfIterators[iteratorNumber].nodePointer);
        listOfIterators[END].nodePointer->numberOfKeys++;
        listOfIterators[END].positionPointer++;
        listOfIterators[END].nodePointer->keys[listOfIterators[END].positionPointer] = key;
        if(listOfIterators[iteratorNumber].nodePointer->prev != nullptr && listOfIterators[iteratorNumber].nodePointer->prev->prev != nullptr)
            mergePairOfAlmostEmptyNodes(listOfIterators[iteratorNumber].nodePointer->prev->prev, listOfIterators[iteratorNumber].nodePointer->prev);
        return;
    }

    // Case 3. Adding key after other key (!= END).
    // Case 3.1. When node is not full.
    if(listOfIterators[iteratorNumber].nodePointer->numberOfKeys < blockSize)
    {
        shiftKeysInNode(listOfIterators[iteratorNumber].nodePointer, Direction::backward, listOfIterators[iteratorNumber].positionPointer + 1);
        listOfIterators[iteratorNumber].nodePointer->keys[listOfIterators[iteratorNumber].positionPointer + 1] = key;
        return;
    }

    // Case 3.2. When node is full.
    splitNode(listOfIterators[iteratorNumber].nodePointer);
    shiftKeysInNode(listOfIterators[iteratorNumber].nodePointer, Direction::backward, listOfIterators[iteratorNumber].positionPointer + 1);
    listOfIterators[BEG].positionPointer = 0;
    listOfIterators[iteratorNumber].nodePointer->keys[listOfIterators[iteratorNumber].positionPointer + 1] = key;

    if(listOfIterators[iteratorNumber].nodePointer->next != nullptr && listOfIterators[iteratorNumber].nodePointer->next->next != nullptr)
        mergePairOfAlmostEmptyNodes(listOfIterators[iteratorNumber].nodePointer->next, listOfIterators[iteratorNumber].nodePointer->next->next);
    if(listOfIterators[iteratorNumber].nodePointer->prev != nullptr && listOfIterators[iteratorNumber].nodePointer->prev->prev != nullptr)
        mergePairOfAlmostEmptyNodes(listOfIterators[iteratorNumber].nodePointer->prev->prev, listOfIterators[iteratorNumber].nodePointer->prev);

    return;
}

void LinkedList::moveIterator(unsigned short int iteratorNumber, Direction direction)
{
    if(direction == Direction::backward && !(listOfIterators[iteratorNumber].nodePointer == listOfIterators[END].nodePointer && listOfIterators[iteratorNumber].positionPointer == listOfIterators[END].positionPointer))
    {
        if(listOfIterators[iteratorNumber].nodePointer->numberOfKeys > listOfIterators[iteratorNumber].positionPointer + 1)
        {
            listOfIterators[iteratorNumber].positionPointer++;
            return;
        }

        listOfIterators[iteratorNumber].nodePointer = listOfIterators[iteratorNumber].nodePointer->next;
        listOfIterators[iteratorNumber].positionPointer = 0;
        return;    
    }

    if(direction == Direction::forward && !(listOfIterators[iteratorNumber].nodePointer == listOfIterators[BEG].nodePointer && listOfIterators[iteratorNumber].positionPointer == listOfIterators[BEG].positionPointer))
    {
        if(listOfIterators[iteratorNumber].positionPointer > firstPosition)
        {
            listOfIterators[iteratorNumber].positionPointer--;
            return;
        }
        
        listOfIterators[iteratorNumber].nodePointer = listOfIterators[iteratorNumber].nodePointer->prev;
        listOfIterators[iteratorNumber].positionPointer = listOfIterators[iteratorNumber].nodePointer->numberOfKeys - 1;
        return;
    }

    if(direction == Direction::uninitialize)
    {
        listOfIterators[iteratorNumber].nodePointer = nullptr;
        listOfIterators[iteratorNumber].positionPointer = 0;
        return;
    }
}

void LinkedList::initializeIterator(unsigned short int newIteratorNumber, unsigned short int oldIteratorNumber)
{
    listOfIterators[newIteratorNumber].nodePointer = listOfIterators[oldIteratorNumber].nodePointer;
    listOfIterators[newIteratorNumber].positionPointer = listOfIterators[oldIteratorNumber].positionPointer;
}

void LinkedList::removeKey(unsigned short int iteratorNumber)
{
    // Case 1. Removing the last key in the list.
    if(listOfIterators[BEG].nodePointer == listOfIterators[END].nodePointer && listOfIterators[BEG].positionPointer == listOfIterators[END].positionPointer && listOfIterators[iteratorNumber].nodePointer == listOfIterators[BEG].nodePointer && listOfIterators[iteratorNumber].positionPointer == listOfIterators[BEG].positionPointer)
    {
        for(unsigned short int i = 0; i < numberOfListIterators - 1; i++)
            moveIterator(i, Direction::uninitialize);

        delete listOfIterators[numberOfListIterators - 1].nodePointer;

        listOfIterators[numberOfListIterators - 1].nodePointer = nullptr;
        listOfIterators[numberOfListIterators - 1].positionPointer = 0;
        return;
    }

    // Case 2. Removing the END key.
    if(listOfIterators[iteratorNumber].nodePointer == listOfIterators[END].nodePointer && listOfIterators[iteratorNumber].positionPointer == listOfIterators[END].positionPointer)
    {
        if(listOfIterators[iteratorNumber].nodePointer->numberOfKeys > 1)
        {
            shiftIteratorsInNode(listOfIterators[iteratorNumber].nodePointer, listOfIterators[iteratorNumber].positionPointer, listOfIterators[iteratorNumber].positionPointer - 1);
            listOfIterators[iteratorNumber].nodePointer->numberOfKeys--;
            if(listOfIterators[iteratorNumber].nodePointer->prev != nullptr && listOfIterators[iteratorNumber].nodePointer->prev->prev != nullptr)
                mergePairOfAlmostEmptyNodes(listOfIterators[iteratorNumber].nodePointer->prev->prev, listOfIterators[iteratorNumber].nodePointer->prev);
            return;
        }
        
        shiftIteratorsToNewNode(listOfIterators[iteratorNumber].nodePointer, listOfIterators[iteratorNumber].positionPointer, listOfIterators[iteratorNumber].nodePointer->prev, listOfIterators[iteratorNumber].nodePointer->prev->numberOfKeys - 1);
        delete listOfIterators[iteratorNumber].nodePointer->next;
        listOfIterators[iteratorNumber].nodePointer->next = nullptr;
        return;
    }

    // Case 3. Removing the BEG key.
    if(listOfIterators[iteratorNumber].nodePointer == listOfIterators[BEG].nodePointer && listOfIterators[iteratorNumber].positionPointer == listOfIterators[BEG].positionPointer)
    {
        shiftKeysInNode(listOfIterators[iteratorNumber].nodePointer, Direction::forward, listOfIterators[BEG].positionPointer);
        if(listOfIterators[iteratorNumber].nodePointer->numberOfKeys == 0)
        {
            shiftIteratorsToNewNode(listOfIterators[iteratorNumber].nodePointer, listOfIterators[iteratorNumber].positionPointer, listOfIterators[iteratorNumber].nodePointer->next, firstPosition);
            delete listOfIterators[iteratorNumber].nodePointer->prev;
            listOfIterators[iteratorNumber].nodePointer->prev = nullptr;
        }

        if(listOfIterators[iteratorNumber].nodePointer->next != nullptr)
            mergePairOfAlmostEmptyNodes(listOfIterators[iteratorNumber].nodePointer, listOfIterators[iteratorNumber].nodePointer->next);

        return;
    }

    // Case 4. Removing other key (!= BEG and != END) when it is the last key in the node.
    if(listOfIterators[iteratorNumber].nodePointer->numberOfKeys == 1)
    {
        Node* tmp = listOfIterators[iteratorNumber].nodePointer;

        if(tmp->prev != nullptr)
            tmp->prev->next = tmp->next;
        if(tmp->next != nullptr)
            tmp->next->prev = tmp->prev;
        
        shiftIteratorsToNewNode(listOfIterators[iteratorNumber].nodePointer, listOfIterators[iteratorNumber].positionPointer, listOfIterators[iteratorNumber].nodePointer->next, firstPosition);

        delete tmp;
        return;
    }

    // Case 5. Removing other key (!= BEG and != END) when it is not the last key in the node.
    shiftKeysInNode(listOfIterators[iteratorNumber].nodePointer, Direction::forward, listOfIterators[iteratorNumber].positionPointer);
    if(listOfIterators[iteratorNumber].positionPointer == listOfIterators[iteratorNumber].nodePointer->numberOfKeys)
        shiftIteratorsToNewNode(listOfIterators[iteratorNumber].nodePointer, listOfIterators[iteratorNumber].positionPointer, listOfIterators[iteratorNumber].nodePointer->next, firstPosition);

    if(listOfIterators[iteratorNumber].nodePointer->next != nullptr)
        mergePairOfAlmostEmptyNodes(listOfIterators[iteratorNumber].nodePointer, listOfIterators[iteratorNumber].nodePointer->next);
    if(listOfIterators[iteratorNumber].nodePointer->prev != nullptr)
        mergePairOfAlmostEmptyNodes(listOfIterators[iteratorNumber].nodePointer->prev, listOfIterators[iteratorNumber].nodePointer);
}

void LinkedList::printList() const
{
    Node* tmp = listOfIterators[BEG].nodePointer;

    while(tmp != nullptr)
    {
        printf("New block: ");
        for(unsigned short int i = 0; i < tmp->numberOfKeys; i++)
            printf("%llu ", tmp->keys[i]);
        
        tmp = tmp->next;
    }
    
    printf("\n");
}

void LinkedList::printList(unsigned short int iteratorNumber) const
{
    printf("%llu\n", listOfIterators[iteratorNumber].nodePointer->keys[listOfIterators[iteratorNumber].positionPointer]);
}