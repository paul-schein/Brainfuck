#pragma once

#define TRUE 1
#define FALSE 0

#define increment(bfInstance) bfInstance->value++
#define decrement(bfInstance) bfInstance->value--

struct structNode  {
    int value;
    int index;
    struct structNode *previous;
    struct structNode *next;
} typedef Node;

void interpret(char *brainfuck, char debug);
char *interpretSection(char *brainfuck, Node **bfInstance, const char debug);
char decrementPointer(Node **bfInstance);
void incrementPointer(Node **bfInstance);
Node *createNewNode();