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

    char *out;
    scanf("%s", &out);
    return 0;
}

void interpret(char *brainfuck, char debug){
    Node *bfInstance = createNewNode();
    if (debug){
        printf("%12s %10s | %10s | %10s ]\n","[", "Value", "Character", "Index");
        printDebugMessage(bfInstance);
    }
    interpretSection(brainfuck, &bfInstance, debug);
}

char *interpretSection(char *brainfuck, Node **bfInstance, char debug){
    char *brainfuckIterator = brainfuck;
    while (*brainfuckIterator != '\0'){
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

        handleDebug(*brainfuckIterator,*bfInstance,debug);
        brainfuckIterator++;
    }
    return NULL;
}

void printDebugMessage(Node *bfInstance){
    printf("%10s", "[");
    DEBUG_COLOR(35)
    printf("%10d | ", bfInstance->value);
    DEBUG_COLOR(31);

    printf("%10s | ", getNonVisualChars(bfInstance));
    DEBUG_COLOR(32)
    printf("%10d ", bfInstance->index);
    DEBUG_COLOR(39)
    printf("]\n");
}

void handleDebug(char current,Node* bfInstance, const char debug) {
    if (debug && (current == '+'
            || current == '-'
            || current == '.'
            || current == ','
            || current == '['
            || current == ']'
            || current == '<'
            || current == '>')){

        switch (current) {
            case '.':
                DEBUG_COLOR(35)
            break;
            case ',':
                DEBUG_COLOR(35)
            break;
            case '+':
                DEBUG_COLOR(34)
            break;
            case '-':
                DEBUG_COLOR(34)
            break;
            case '>':
                DEBUG_COLOR(33)
            break;
            case '<':
                DEBUG_COLOR(33)
            break;
            case '[':
                DEBUG_COLOR(32)
            break;
            case ']':
                DEBUG_COLOR(32)
            break;
            default:
                break;
        }

        printf("%c ", current);
        DEBUG_COLOR(39)
        printDebugMessage(bfInstance);
    }

}

char *getNonVisualChars(Node *bfInstance){
    char bfValue = (char)bfInstance->value;
    char *nonVisualChars[] = {
        "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS", "TAB",
        "LF", "VT", "FF", "CR", "SO", "SI", "DLE", "DC1", "DC2", "DC3",
        "DC4", "NAK", "SYN", "ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS",
        "RS", "US"
    };

    if ((int)bfValue >= 0 && (int)bfValue <= 31) {
        return nonVisualChars[(int)bfValue];
    } else {
        return (char *)&bfInstance->value;
    }
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
    size_t bytesRead = 0;
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