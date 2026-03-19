#ifndef TOKEN_H
#define TOKEN_H

typedef enum {

    // Keywords
    TOKEN_INT,
    TOKEN_PRINT,

    // Identifiers & literals
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,

    // Operators
    TOKEN_ASSIGN,     // <-
    TOKEN_PLUS,       // +
    TOKEN_MINUS,      // -
    TOKEN_MUL,        // *
    TOKEN_DIV,        // /

    // Symbols
    TOKEN_LPAREN,     // (
    TOKEN_RPAREN,     // )
    TOKEN_SEMICOLON,  // ;

    // Special
    TOKEN_EOF,
    TOKEN_UNKNOWN

} TokenType;

typedef struct {
    TokenType type;
    char value[64];
} Token;

const char* token_type_to_string(TokenType type);
#endif