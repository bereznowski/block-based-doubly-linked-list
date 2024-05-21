#include <iostream>
#include <string.h>
#include "LinkedList.h"

#define bufferLength 100

using namespace std;

short int charToShortInt(char c)
{
    short int n = c - '0';

    return n;
}

unsigned long long int charToUnsignedLongLongInt(char* c)
{
    unsigned long long int n = c[0] - '0';
    int i = 1;

    while(c[i] != '\0')
    {
        n = n * 10 + c[i] - '0';
        i++;
    }

    return n;
}

size_t charToSize(char* c)
{
    size_t n = c[0] - '0';
    int i = 1;

    while(c[i] != '\0')
    {
        n = n * 10 + c[i] - '0';
        i++;
    }

    return n;
}

void executeCommand(char command[], LinkedList& list)
{
    char command1[bufferLength], command2[bufferLength];

    scanf("%99s", command1);

    if(strcmp(command, "i") == 0 || strcmp(command, ".A") == 0 || strcmp(command, "A.") == 0)
        scanf("%99s", command2);

    if(strcmp(command, "i") == 0)
    {
        short int newIterator = charToShortInt(command1[0]);

        if(strcmp(command2, "BEG") == 0)
            list.initializeIterator(newIterator, BEG);
        else if(strcmp(command2, "END") == 0)
            list.initializeIterator(newIterator, END);
        else
        {
            short int oldIterator = charToShortInt(command2[0]);
            list.initializeIterator(newIterator, oldIterator);
        }
        return;
    }

    if(strcmp(command, "+") == 0)
    {
        short int iteratorNumber = charToShortInt(command1[0]);
        list.moveIterator(iteratorNumber, Direction::backward);
        return;
    }

    if(strcmp(command, "-") == 0)
    {
        short int iteratorNumber = charToShortInt(command1[0]);
        list.moveIterator(iteratorNumber, Direction::forward);
        return;
    }

    if(strcmp(command, ".A") == 0)
    {
        unsigned long long int newKey = charToUnsignedLongLongInt(command2);

        if(strcmp(command1, "BEG") == 0)
            list.addKeyBefore(BEG, newKey);
            
        else if(strcmp(command1, "END") == 0)
            list.addKeyBefore(END, newKey);
        else
        {
            short int iteratorNumber = charToShortInt(command1[0]);
            list.addKeyBefore(iteratorNumber, newKey);
        }
        return;
    }

    if(strcmp(command, "A.") == 0)
    {
        unsigned long long int newKey = charToUnsignedLongLongInt(command2);

        if(strcmp(command1, "BEG") == 0)
            list.addKeyAfter(BEG, newKey);
        else if(strcmp(command1, "END") == 0)
            list.addKeyAfter(END, newKey);
        else
        {
            short int iteratorNumber = charToShortInt(command1[0]);
            list.addKeyAfter(iteratorNumber, newKey);
        }
        return;
    }

    if(strcmp(command, "R") == 0)
    {
        if(strcmp(command1, "BEG") == 0)
            list.removeKey(BEG);
        else if(strcmp(command1, "END") == 0)
            list.removeKey(END);
        else
        {
            short int iteratorNumber = charToShortInt(command1[0]);
            list.removeKey(iteratorNumber);
        }
        return;
    }

    if(strcmp(command, "P") == 0)
    {
        if(strcmp(command1, "ALL") == 0)
            list.printList();
        else
        {
            short int currentIterator = charToShortInt(command1[0]);
            list.printList(currentIterator);
        }
        return;
    }
}

int main()
{
    char command[bufferLength];

    scanf("%99s %99s", command, command);

    unsigned short int size = (charToSize(command) - sizeof(Node*) * 2 - sizeof(unsigned short int)) / sizeof(unsigned long long int);

    LinkedList list(size);

    while(true)
    {
        scanf("%99s", command);

        if(feof(stdin) != 0)
            break;

        executeCommand(command, list);
    }
}