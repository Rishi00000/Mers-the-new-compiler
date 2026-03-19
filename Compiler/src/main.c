#include <stdio.h>
#include "../include/parser.h"

int main(int argc, char *argv[]) {

    printf("Program started\n");

    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *source = fopen(argv[1], "r");

    if (!source) {
        printf("Error opening file\n");
        return 1;
    }

    ASTNode* root = parse(source);

    if (root) {
        printf("Parsing successful\n");
    } else {
        printf("Parsing failed\n");
    }

    fclose(source);
    return 0;
}