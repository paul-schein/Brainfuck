#include "brainfuckInterpreter.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        return 1;
    }
    unsigned int length = getFileLength(argv[1]);
    if (length <= 0) {
        return 1;
    }

    char content[length + 1];
    memset(content, 0, length + 1);
    readFile(argv[1], content, sizeof(content));

    interpret(content, argc > 2 && argv[2][0] == '-' && argv[2][1] == 'd');

    char *out;
    scanf("%s", &out);
    return 0;
}

void interpret(char *brainfuck, char debug) {
    Node *bfInstance = createNewNode();
    if (debug) {
        printf("  [Value | Character | Index]\n  ");
        printDebugMessage(bfInstance);
    }
    interpretSection(brainfuck, &bfInstance, debug);
}

char *interpretSection(char *brainfuck, Node **bfInstance, char debug) {
    char *brainfuckIterator = brainfuck;
    while (*brainfuckIterator != '\0') {
        switch (*brainfuckIterator) {
            case '.':
                printf("%c", (*bfInstance)->value);
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
                if (!decrementPointer(bfInstance)) {
                    printf("\nOut of Range: Index smaller than 0\n");
                    return NULL;
                }
                break;
            case '[':
                brainfuckIterator = interpretSection(brainfuckIterator + 1, bfInstance, debug);
                if (brainfuckIterator == NULL) {
                    return NULL;
                }
                break;
            case ']':
                if ((*bfInstance)->value == 0) {
                    return brainfuckIterator;
                }
                brainfuckIterator = brainfuck - 1;
                break;
        }
        if (debug && (
            *brainfuckIterator == '+'
            || *brainfuckIterator == '-'
            || *brainfuckIterator == '.'
            || *brainfuckIterator == ','
            || *brainfuckIterator == '['
            || *brainfuckIterator == ']'
            || *brainfuckIterator == '<'
            || *brainfuckIterator == '>')) {
            printf("%c ", *brainfuckIterator);
            printDebugMessage(*bfInstance);
        }
        brainfuckIterator++;
    }
    return NULL;
}

void printDebugMessage(Node *bfInstance) {
    printf("[%d | %c | %d]\n", bfInstance->value, bfInstance->value, bfInstance->index);
}

Node *createNewNode() {
    Node *newNode = malloc(sizeof(Node));
    newNode->value = 0;
    newNode->index = 0;
    newNode->next = NULL;
    newNode->previous = NULL;
    return newNode;
}

void incrementPointer(Node **bfInstance) {
    if ((*bfInstance)->next != NULL) {
        *bfInstance = (*bfInstance)->next;
        return;
    }
    (*bfInstance)->next = createNewNode();
    (*bfInstance)->next->index = (*bfInstance)->index + 1;
    (*bfInstance)->next->previous = *bfInstance;
    *bfInstance = (*bfInstance)->next;
}

char decrementPointer(Node **bfInstance) {
    if ((*bfInstance)->previous == NULL) {
        return FALSE;
    }
    *bfInstance = (*bfInstance)->previous;
    return TRUE;
}

size_t getFileLength(char *path) {
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        return 0;
    }

    char buffer[100];
    size_t length = 0;
    size_t bytesRead = 0;
    while ((bytesRead = fread(buffer, sizeof(char), 100, file))) {
        length += bytesRead;
    }
    return length;
}

void readFile(char *path, char *content, size_t length) {
    FILE *file = fopen(path, "r");

    if (file == NULL
    || content == NULL) {
        return;
    }

    fread(content, sizeof(char), length, file);

    fclose(file);
}