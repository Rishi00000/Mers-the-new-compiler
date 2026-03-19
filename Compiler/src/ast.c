#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ast.h"

ASTNode* create_node(ASTNodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));

    node->type = type;
    node->name[0] = '\0';
    node->value = 0;
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;

    return node;
}

// Helper for indentation
void indent(int level) {
    for (int i = 0; i < level; i++)
        printf("  ");
}

void print_ast(ASTNode* node, int level) {
    if (!node) return;

    indent(level);

    switch(node->type) {
        case AST_PROGRAM: printf("PROGRAM\n"); break;
        case AST_DECLARATION: printf("DECL(%s)\n", node->name); break;
        case AST_ASSIGNMENT: printf("ASSIGN(%s)\n", node->name); break;
        case AST_PRINT: printf("PRINT\n"); break;
        case AST_NUMBER: printf("NUMBER(%d)\n", node->value); break;
        case AST_VARIABLE: printf("VAR(%s)\n", node->name); break;
        case AST_BINARY: printf("BINARY\n"); break;
    }

    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);

    // traverse next statements
    print_ast(node->next, level);
}