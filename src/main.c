#include <stdio.h>
#include <string.h>
#include "util.h"
#include "brainfuckInterpreter.h"
#include "brainfuckDebugUtil.h"

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