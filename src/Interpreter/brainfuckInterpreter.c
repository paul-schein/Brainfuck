#include <stdio.h>
#include <stdlib.h>
#include "brainfuckInterpreter.h"
#include "brainfuckDebugUtil.h"

void interpret(char *brainfuck, const char debug){
    Node *bfInstance = createNewNode();
    if (debug){
        printf("%12s %10s | %10s | %10s ]\n","[", "Value", "Character", "Index");
    }
    interpretSection(brainfuck, &bfInstance, debug);
}

char *interpretSection(char *brainfuck, Node **bfInstance, const char debug){
    char *brainfuckIterator = brainfuck;
    while (*brainfuckIterator != '\0'){
        handleDebugSymbol(*brainfuckIterator, debug);
        switch (*brainfuckIterator){
        case '.':
            if(!debug){
                printf("%c", (*bfInstance)->value);
            }
            break;
        case ',':
            (*bfInstance)->value = getchar();
            break;
        case '+':
            increment((*bfInstance));
            break;
        case '-':
            decrement((*bfInstance));
            break;
        case '>':
            incrementPointer(bfInstance);
            break;
        case '<':
            if (!decrementPointer(bfInstance)){
                DEBUG_COLOR(31)
                printf("\nOut of Range: Index smaller than 0\n");
                return NULL;
            }
            break;
        case '[':
            brainfuckIterator = interpretSection(brainfuckIterator + 1, bfInstance, debug);
            if (brainfuckIterator == NULL){
                return NULL;
            }
            break;
        case ']':
            if ((*bfInstance)->value == 0){
                return brainfuckIterator;
            }
            brainfuckIterator = brainfuck - 1;
            break;
        default:
            break;
        }
        handleDebugMessage(*bfInstance,debug);
        brainfuckIterator++;
    }
    return NULL;
}

Node *createNewNode(){
    Node *newNode = malloc(sizeof(Node));
    newNode->value = 0;
    newNode->index = 0;
    newNode->next = NULL;
    newNode->previous = NULL;
    return newNode;
}

void incrementPointer(Node **bfInstance){
    if ((*bfInstance)->next != NULL)
    {
        *bfInstance = (*bfInstance)->next;
        return;
    }
    (*bfInstance)->next = createNewNode();
    (*bfInstance)->next->index = (*bfInstance)->index + 1;
    (*bfInstance)->next->previous = *bfInstance;
    *bfInstance = (*bfInstance)->next;
}

char decrementPointer(Node **bfInstance){
    if ((*bfInstance)->previous == NULL)
    {
        return FALSE;
    }
    *bfInstance = (*bfInstance)->previous;
    return TRUE;
}