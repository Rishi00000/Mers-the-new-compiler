#ifndef TOKEN_H
#define TOKEN_H

typedef enum {

    // Keywords
    TOKEN_INT,
    TOKEN_PRINT,
    TOKEN_IF,
    TOKEN_ELSE,

    // Identifiers & literals
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,

    // Operators
    TOKEN_ASSIGN,     // <-
    TOKEN_PLUS,       // +
    TOKEN_MINUS,      // -
    TOKEN_MUL,        // *
    TOKEN_DIV,        // /

    // Comparison operators
    TOKEN_LT,         // <
    TOKEN_GT,         // >
    TOKEN_LE,         // <=
    TOKEN_GE,         // >=
    TOKEN_EQ,         // ==
    TOKEN_NE,         // !=

    // Symbols
    TOKEN_LPAREN,     // (
    TOKEN_RPAREN,     // )
    TOKEN_LBRACE,     // {
    TOKEN_RBRACE,     // }
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