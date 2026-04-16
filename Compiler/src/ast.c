#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ast.h"

ASTNode* create_node(ASTNodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));

    node->type = type;
    node->name[0] = '\0';
    node->value = 0;
    node->operator = 0;
    node->left = NULL;
    node->right = NULL;
    node->condition = NULL;
    node->true_branch = NULL;
    node->false_branch = NULL;
    node->next = NULL;

    return node;
}

// Helper to count children
int count_children(ASTNode* node) {
    int count = 0;
    if (node->left) count++;
    if (node->right) count++;
    if (node->condition) count++;
    if (node->true_branch) count++;
    if (node->false_branch) count++;
    if (node->next) count++;
    return count;
}

// Helper to print node label
void print_node_label(ASTNode* node) {
    switch(node->type) {
        case AST_PROGRAM: printf("PROGRAM"); break;
        case AST_DECLARATION: printf("DECL(%s)", node->name); break;
        case AST_ASSIGNMENT: printf("ASSIGN(%s)", node->name); break;
        case AST_PRINT: printf("PRINT"); break;
        case AST_NUMBER: printf("NUMBER(%d)", node->value); break;
        case AST_VARIABLE: printf("VAR(%s)", node->name); break;
        case AST_BINARY: printf("BINARY"); break;
        case AST_COMPARISON: printf("COMPARISON"); break;
        case AST_IF: printf("IF"); break;
    }
}

// Recursive helper to print tree with proper formatting
void print_ast_helper(ASTNode* node, const char* prefix, int is_last) {
    if (!node) return;

    printf("%s", prefix);
    printf("%s", is_last ? "+-- " : "|-- ");
    print_node_label(node);
    printf("\n");

    // Prepare prefix for children
    const char* extension = is_last ? "    " : "|   ";
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, extension);

    // Count actual children to determine last child
    int child_count = 0;
    ASTNode* children[6];
    
    if (node->left) children[child_count++] = node->left;
    if (node->right) children[child_count++] = node->right;
    if (node->condition) children[child_count++] = node->condition;
    if (node->true_branch) children[child_count++] = node->true_branch;
    if (node->false_branch) children[child_count++] = node->false_branch;
    if (node->next) children[child_count++] = node->next;

    // Print all children
    for (int i = 0; i < child_count; i++) {
        print_ast_helper(children[i], new_prefix, (i == child_count - 1));
    }
}

void print_ast(ASTNode* node, int level) {
    if (!node) return;

    // Print root node
    print_node_label(node);
    printf("\n");

    // Count children for root
    int child_count = 0;
    ASTNode* children[6];
    
    if (node->left) children[child_count++] = node->left;
    if (node->right) children[child_count++] = node->right;
    if (node->condition) children[child_count++] = node->condition;
    if (node->true_branch) children[child_count++] = node->true_branch;
    if (node->false_branch) children[child_count++] = node->false_branch;
    if (node->next) children[child_count++] = node->next;

    // Print all children
    for (int i = 0; i < child_count; i++) {
        print_ast_helper(children[i], "", (i == child_count - 1));
    }
}