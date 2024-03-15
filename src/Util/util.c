#include "util.h"

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