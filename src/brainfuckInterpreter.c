#include "brainfuckInterpreter.h"
#include <stdio.h>
#include <stdlib.h>

int main(const int argc, char **argv) {
    DEBUG_COLOR(1)
    if (argc < 2) {
        return 1;
    }
    const unsigned int length = getFileLength(argv[1]);
    if (length <= 0) {
        return 1;
    }

    char content[length + 1];
    memset(content, 0, length + 1);
    readFile(argv[1], content, sizeof(content));

    interpret(content, argc > 2 && argv[2][0] == '-' && argv[2][1] == 'd');

    char *out = "";
    printf("\n");
    scanf("%s", out);
    return 0;
}

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

void printDebugMessage(Node *bfInstance){
    printf(" %10s", "[");
    DEBUG_COLOR(35)
    printf("%10d | ", bfInstance->value);
    DEBUG_COLOR(31);

    printf("%10s | ", getNonVisualChars(bfInstance));
    DEBUG_COLOR(32)
    printf("%10d ", bfInstance->index);
    DEBUG_COLOR(39)
    printf("]");
}

void handleDebugMessage(Node *bfInstance, const char debug) {
    if (debug) {
        printDebugMessage(bfInstance);
    }
}

void handleDebugSymbol(const char symbol, const char debug) {
    if (!debug || !isRelevantSymbol(symbol)) {
        return;
    }
    switch (symbol) {
        case '.':
        case ',':
            DEBUG_COLOR(35)
        break;
        case '+':
        case '-':
            DEBUG_COLOR(34)
        break;
        case '>':
        case '<':
            DEBUG_COLOR(33)
        break;
        case '[':
        case ']':
            DEBUG_COLOR(32)
        break;
        default:
            break;
    }

    printf("\n%c", symbol);
    DEBUG_COLOR(39)
}

char isRelevantSymbol(const char symbol) {
    return symbol == '+'
            || symbol == '-'
            || symbol == '.'
            || symbol == ','
            || symbol == '['
            || symbol == ']'
            || symbol == '<'
            || symbol == '>';
}

char *getNonVisualChars(Node *bfInstance){
    const char bfValue = bfInstance->value;
    char *nonVisualChars[] = {
        "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS", "TAB",
        "LF", "VT", "FF", "CR", "SO", "SI", "DLE", "DC1", "DC2", "DC3",
        "DC4", "NAK", "SYN", "ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS",
        "RS", "US"
    };

    return (int)bfValue >= 0 && (int)bfValue <= 31
    ? nonVisualChars[(int)bfValue]
    : (char *)&bfInstance->value;
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

size_t getFileLength(const char *path){
    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        return 0;
    }

    char buffer[100];
    size_t length = 0;
    size_t bytesRead;
    while ((bytesRead = fread(buffer, sizeof(char), 100, file)))
    {
        length += bytesRead;
    }
    return length;
}

void readFile(const char *path, char *content, const size_t length){
    FILE *file = fopen(path, "r");

    if (file == NULL || content == NULL)
    {
        return;
    }

    fread(content, sizeof(char), length, file);

    fclose(file);
}