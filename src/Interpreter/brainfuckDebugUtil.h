#pragma once

#include "brainfuckInterpreter.h"

#define DEBUG_COLOR(COL) printf("\x1b[%dm",COL);

void printDebugMessage(Node *bfInstance);
char* getNonVisualChars(Node * bfInstance);
void handleDebugSymbol(const char symbol, const char debug);
void handleDebugMessage(Node *bfInstance, const char debug);
char isRelevantSymbol(const char symbol);