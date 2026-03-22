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

    // First operand
    if (current_token.type == TOKEN_NUMBER)
        left = parse_number();
    else
        left = parse_variable();

    // Handle multiple operators: +, -, *, /
    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS ||
           current_token.type == TOKEN_MUL || current_token.type == TOKEN_DIV) {

        ASTNode* node = create_node(AST_BINARY);

        node->operator = current_token.type;  // Store the operator
        advance(); // consume operator

        node->left = left;

        // Right operand
        if (current_token.type == TOKEN_NUMBER)
            node->right = parse_number();
        else
            node->right = parse_variable();

        left = node; // IMPORTANT: chaining
    }

    return left;
}

// ---------- COMPARISON EXPRESSIONS ----------

// Forward declaration for recursive use in parse_block
ASTNode* parse_statement();

ASTNode* parse_comparison() {
    ASTNode* left = parse_expression();

    // If we have a comparison operator, create a comparison node
    if (current_token.type == TOKEN_LT || current_token.type == TOKEN_GT ||
        current_token.type == TOKEN_LE || current_token.type == TOKEN_GE ||
        current_token.type == TOKEN_EQ || current_token.type == TOKEN_NE) {

        ASTNode* comp = create_node(AST_COMPARISON);
        comp->operator = current_token.type;
        advance(); // consume operator

        comp->left = left;
        comp->right = parse_expression();

        return comp;
    }

    return left;
}

// ---------- STATEMENTS ----------

// Parse a block: { stmt1; stmt2; ... }
ASTNode* parse_block() {
    if (current_token.type != TOKEN_LBRACE) {
        error("Expected '{'");
        return NULL;
    }

    advance(); // skip '{'

    ASTNode* head = NULL;
    ASTNode* current = NULL;

    while (current_token.type != TOKEN_RBRACE && current_token.type != TOKEN_EOF) {

        ASTNode* stmt = parse_statement();

        if (!stmt) {
            error("Statement parsing failed in block");
            return NULL;
        }

        if (!head) {
            head = stmt;
            current = stmt;
        } else {
            current->next = stmt;
            current = stmt;
        }
    }

    if (current_token.type != TOKEN_RBRACE) {
        error("Expected '}' to close block");
        return NULL;
    }

    advance(); // skip '}'

    return head;
}

// Parse if-else statement
ASTNode* parse_if_statement() {
    ASTNode* node = create_node(AST_IF);

    advance(); // skip 'if'

    if (current_token.type != TOKEN_LPAREN) {
        error("Expected '(' after if");
        return NULL;
    }

    advance();

    node->condition = parse_comparison();

    if (current_token.type != TOKEN_RPAREN) {
        error("Expected ')' after condition");
        return NULL;
    }

    advance();

    node->true_branch = parse_block();

    if (current_token.type == TOKEN_ELSE) {
        advance(); // skip 'else'
        node->false_branch = parse_block();
    }

    return node;
}

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

    if (current_token.type == TOKEN_IF)
        return parse_if_statement();

    if (current_token.type == TOKEN_IDENTIFIER)
        return parse_assignment();

    error("Unknown statement");
    return NULL;
}

// ---------- PROGRAM ----------

ASTNode* parse(FILE *source) {

    src = source;
    advance();

    ASTNode* program = create_node(AST_PROGRAM);

    ASTNode* head = NULL;
    ASTNode* current = NULL;

    while (current_token.type != TOKEN_EOF) {

        ASTNode* stmt = parse_statement();

        if (!stmt) {
            printf("Statement parsing failed\n");
            return NULL;
        }

        if (!head) {
            head = stmt;
            current = stmt;
        } else {
            current->next = stmt;
            current = stmt;
        }
    }

    program->left = head; // attach statements to program
    return program;
}