#include <stdio.h>
#include "brainfuckDebugUtil.h"

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