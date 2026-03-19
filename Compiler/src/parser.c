#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/parser.h"
#include "../include/lexer.h"

Token current_token;
FILE *src;

void advance() {
    current_token = get_next_token(src);
    // printf("Token: %d\n", current_token.type); // DEBUG
}

void error(const char *msg) {
    printf("Parser Error: %s\n", msg);
}

// ---------- BASIC NODES ----------

ASTNode* parse_number() {
    ASTNode* node = create_node(AST_NUMBER);
    node->value = atoi(current_token.value);
    advance();
    return node;
}

ASTNode* parse_variable() {
    ASTNode* node = create_node(AST_VARIABLE);
    strcpy(node->name, current_token.value);
    advance();
    return node;
}

// ---------- EXPRESSIONS ----------

ASTNode* parse_expression() {
    ASTNode* left;

    if (current_token.type == TOKEN_NUMBER)
        left = parse_number();
    else
        left = parse_variable();

    if (current_token.type == TOKEN_MINUS) { // your addition
        ASTNode* node = create_node(AST_BINARY);
        advance();

        node->left = left;

        if (current_token.type == TOKEN_NUMBER)
            node->right = parse_number();
        else
            node->right = parse_variable();

        return node;
    }

    return left;
}

// ---------- STATEMENTS ----------

ASTNode* parse_declaration() {
    ASTNode* node = create_node(AST_DECLARATION);

    advance(); // skip 'int'

    if (current_token.type != TOKEN_IDENTIFIER) {
        error("Expected identifier after int");
        return NULL;
    }

    strcpy(node->name, current_token.value);
    advance();

    if (current_token.type != TOKEN_SEMICOLON) {
        error("Expected ';' after declaration");
        return NULL;
    }

    advance();
    return node;
}

ASTNode* parse_assignment() {
    ASTNode* node = create_node(AST_ASSIGNMENT);

    strcpy(node->name, current_token.value);
    advance();

    if (current_token.type != TOKEN_ASSIGN) {
        error("Expected <-");
        return NULL;
    }

    advance();

    node->left = parse_expression();

    if (current_token.type != TOKEN_SEMICOLON) {
        error("Expected ';'");
        return NULL;
    }

    advance();
    return node;
}

ASTNode* parse_print() {
    ASTNode* node = create_node(AST_PRINT);

    advance(); // skip 'print'

    if (current_token.type != TOKEN_LPAREN) {
        error("Expected '(' after print");
        return NULL;
    }

    advance();

    node->left = parse_expression();

    if (current_token.type != TOKEN_RPAREN) {
        error("Expected ')'");
        return NULL;
    }

    advance();

    if (current_token.type != TOKEN_SEMICOLON) {
        error("Expected ';'");
        return NULL;
    }

    advance();

    return node;
}

// ---------- MAIN STATEMENT HANDLER ----------

ASTNode* parse_statement() {

    if (current_token.type == TOKEN_INT)
        return parse_declaration();

    if (current_token.type == TOKEN_PRINT)
        return parse_print();

    if (current_token.type == TOKEN_IDENTIFIER)
        return parse_assignment();

    error("Unknown statement");
    return NULL;
}

// ---------- PROGRAM ----------

ASTNode* parse(FILE *source) {

    // printf("Entered parser\n");

    src = source;
    advance();

    while (current_token.type != TOKEN_EOF) {

        // printf("Parsing statement...\n"); // DEBUG

        ASTNode* stmt = parse_statement();

        if (!stmt) {
            printf("Statement parsing failed\n");
            return NULL;
        }
    }

    return (ASTNode*)1; // temporary success
}