#include <string.h>

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

size_t getFileLength(char *path);
void readFile(char *path, char *content, size_t length);
void interpret(char *brainfuck, char debug);
char *interpretSection(char *brainfuck, Node **bfInstance, char debug);
char decrementPointer(Node **bfInstance);
void incrementPointer(Node **bfInstance);
void printDebugMessage(Node *bfInstance);
Node *createNewNode();