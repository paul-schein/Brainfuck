#pragma once

#include <stdio.h>

size_t getFileLength(const char *path);
void readFile(const char *path, char *content, const size_t length);