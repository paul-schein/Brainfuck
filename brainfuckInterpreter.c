#include "brainfuckInterpreter.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        return 1;
    }
    unsigned int length = getFileLength(argv[1]);
    if (length <= 0)
    {
        return 1;
    }

    char content[length + 1];
    memset(content, 0, length + 1);
    readFile(argv[1], content, sizeof(content));

    interpret(content, argc > 2 && argv[2][0] == '-' && argv[2][1] == 'd');

    return 0;
}

void interpret(char *brainfuck, char debug)
{
    Node *bfInstance = createNewNode();
    if (debug)
    {
        printf("  [Value | Character | Index]\n  ");
        printDebugMessage(bfInstance);
    }
    interpretSection(brainfuck, &bfInstance, debug);
}

char *interpretSection(char *brainfuck, Node **bfInstance, char debug)
{
    char *brainfuckIterator = brainfuck;
    while (*brainfuckIterator != '\0')
    {
        switch (*brainfuckIterator)
        {
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
            if (!decrementPointer(bfInstance))
            {
                printf("\nOut of Range: Index smaller than 0\n");
                return NULL;
            }
            break;
        case '[':
            brainfuckIterator = interpretSection(brainfuckIterator + 1, bfInstance, debug);
            if (brainfuckIterator == NULL)
            {
                return NULL;
            }
            break;
        case ']':
            if ((*bfInstance)->value == 0)
            {
                return brainfuckIterator;
            }
            brainfuckIterator = brainfuck - 1;
            break;
        }
        if (debug && (*brainfuckIterator == '+' || *brainfuckIterator == '-' || *brainfuckIterator == '.' || *brainfuckIterator == ',' || *brainfuckIterator == '[' || *brainfuckIterator == ']' || *brainfuckIterator == '<' || *brainfuckIterator == '>'))
        {
            printf("%c ", *brainfuckIterator);
            printDebugMessage(*bfInstance);
        }
        brainfuckIterator++;
    }
    return NULL;
}

void printDebugMessage(Node *bfInstance)
{
    printf(" %10s", "[");
    DEBUG_COLOR(35)
    printf(" %d | ", bfInstance->value);
    DEBUG_COLOR(31);

    printf("%s | ", getNonVisualChars(bfInstance));
    DEBUG_COLOR(32)
    printf("%d ", bfInstance->index);
    DEBUG_COLOR(39)
    printf("]\n");
}

char *getNonVisualChars(Node *bfInstance)
{
    char bfValue = (char)bfInstance->value;
    char *outputVal = "";
    switch ((int)bfValue)
    {
    case 0:
        outputVal = "NUL";
        break;
    case 1:
        outputVal = "SOH";
        break;
    case 2:
        outputVal = "STX";
        break;
    case 3:
        outputVal = "ETX";
        break;
    case 4:
        outputVal = "EOT";
        break;
    case 5:
        outputVal = "ENQ";
        break;
    case 6:
        outputVal = "ACK";
        break;
    case 7:
        outputVal = "BEL";
        break;
    case 8:
        outputVal = "BS";
        break;
    case 9:
        outputVal = "TAB";
        break;
    case 10:
        outputVal = "LF";
        break;
    case 11:
        outputVal = "VT";
        break;
    case 12:
        outputVal = "FF";
        break;
    case 13:
        outputVal = "CR";
        break;
    case 14:
        outputVal = "SO";
        break;
    case 15:
        outputVal = "SI";
        break;
    case 16:
        outputVal = "DLE";
        break;
    case 17:
        outputVal = "DC1";
        break;
    case 18:
        outputVal = "DC2";
        break;
    case 19:
        outputVal = "DC3";
        break;
    case 20:
        outputVal = "DC4";
        break;
    case 21:
        outputVal = "NAK";
        break;
    case 22:
        outputVal = "SYN";
        break;
    case 23:
        outputVal = "ETB";
        break;
    case 24:
        outputVal = "CAN";
        break;
    case 25:
        outputVal = "EM";
        break;
    case 26:
        outputVal = "SUB";
        break;
    case 27:
        outputVal = "ESC";
        break;
    case 28:
        outputVal = "FS";
        break;
    case 29:
        outputVal = "GS";
        break;
    case 30:
        outputVal = "RS";
        break;
    case 31:
        outputVal = "US";
        break;
    default:
        outputVal = (char *)&bfInstance->value;
        break;
    }

    return outputVal;
}

Node *createNewNode()
{
    Node *newNode = malloc(sizeof(Node));
    newNode->value = 0;
    newNode->index = 0;
    newNode->next = NULL;
    newNode->previous = NULL;
    return newNode;
}

void incrementPointer(Node **bfInstance)
{
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

char decrementPointer(Node **bfInstance)
{
    if ((*bfInstance)->previous == NULL)
    {
        return FALSE;
    }
    *bfInstance = (*bfInstance)->previous;
    return TRUE;
}

size_t getFileLength(char *path)
{
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

void readFile(char *path, char *content, size_t length)
{
    FILE *file = fopen(path, "r");

    if (file == NULL || content == NULL)
    {
        return;
    }

    fread(content, sizeof(char), length, file);

    fclose(file);
}