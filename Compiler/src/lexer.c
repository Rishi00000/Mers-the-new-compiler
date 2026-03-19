#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../include/lexer.h"

Token create_token(TokenType type, const char *value) {
    Token token;
    token.type = type;
    strcpy(token.value, value);
    return token;
}

Token get_next_token(FILE *source) {

    int c;

    while ((c = fgetc(source)) != EOF) {

        // Skip whitespace
        if (isspace(c))
            continue;

        // IDENTIFIER or KEYWORD
        if (isalpha(c)) {
            char buffer[64];
            int i = 0;

            buffer[i++] = c;

            while (isalnum(c = fgetc(source)))
                buffer[i++] = c;

            buffer[i] = '\0';
            ungetc(c, source);

            if (strcmp(buffer, "int") == 0)
                return create_token(TOKEN_INT, buffer);

            if (strcmp(buffer, "print") == 0)
                return create_token(TOKEN_PRINT, buffer);

            return create_token(TOKEN_IDENTIFIER, buffer);
        }

        // NUMBER
        if (isdigit(c)) {
            char buffer[64];
            int i = 0;

            buffer[i++] = c;

            while (isdigit(c = fgetc(source)))
                buffer[i++] = c;

            buffer[i] = '\0';
            ungetc(c, source);

            return create_token(TOKEN_NUMBER, buffer);
        }

        // ASSIGN <-
        if (c == '<') {
            int next = fgetc(source);
            if (next == '-')
                return create_token(TOKEN_ASSIGN, "<-");

            printf("Lexer Error: Expected '-' after '<'\n");
            return create_token(TOKEN_UNKNOWN, "");
        }

        // Operators
        if (c == '+') return create_token(TOKEN_PLUS, "+");
        if (c == '-') return create_token(TOKEN_MINUS, "-");
        if (c == '*') return create_token(TOKEN_MUL, "*");
        if (c == '/') return create_token(TOKEN_DIV, "/");

        // Symbols
        if (c == '(') return create_token(TOKEN_LPAREN, "(");
        if (c == ')') return create_token(TOKEN_RPAREN, ")");
        if (c == ';') return create_token(TOKEN_SEMICOLON, ";");

        // Unknown character
        printf("Lexer Error: Unknown character '%c'\n", c);
        return create_token(TOKEN_UNKNOWN, "");
    }

    return create_token(TOKEN_EOF, "");
}