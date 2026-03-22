#include <stdio.h>
#include <string.h>
#include "../include/semantic.h"
#include "../include/symbol_table.h"

SymbolTable table;

void check_node(ASTNode* node) {

    if (!node) return;

    switch(node->type) {

        case AST_DECLARATION:
            if (!add_symbol(&table, node->name)) {
                printf("Semantic Error: Variable '%s' already declared\n", node->name);
            }
            break;

        case AST_ASSIGNMENT:
            if (!find_symbol(&table, node->name)) {
                printf("Semantic Error: Variable '%s' not declared\n", node->name);
            }
            break;

        case AST_VARIABLE:
            if (!find_symbol(&table, node->name)) {
                printf("Semantic Error: Variable '%s' not declared\n", node->name);
            }
            break;

        case AST_COMPARISON:
            // Check both sides of comparison
            check_node(node->left);
            check_node(node->right);
            return; // Skip default traversal for comparison

        case AST_IF:
            // Check condition
            check_node(node->condition);
            // Check true branch
            check_node(node->true_branch);
            // Check false branch
            check_node(node->false_branch);
            return; // Skip default traversal for if

        default:
            break;
    }

    check_node(node->left);
    check_node(node->right);
    check_node(node->next);
}

void analyze(ASTNode* root) {
    init_table(&table);
    check_node(root);
}