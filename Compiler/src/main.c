#include <stdio.h>
#include "../include/lexer.h"
#include "../include/token.h"

extern const char* token_type_to_string(TokenType type);

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *source = fopen(argv[1], "r");

    if (!source) {
        printf("Error: Cannot open file\n");
        return 1;
    }

    Token token;

    do {
        token = get_next_token(source);

        printf("%s", token_type_to_string(token.type));

        if (token.type == TOKEN_IDENTIFIER || token.type == TOKEN_NUMBER)
            printf("(%s)", token.value);

        printf("\n");

    } while (token.type != TOKEN_EOF);

    fclose(source);
    return 0;
}