#include <stdio.h>
#include "../include/ir.h"

int temp_count = 0;

// generate temporary variables like t1, t2...
char* new_temp() {
    static char buffer[20];
    sprintf(buffer, "t%d", temp_count++);
    return buffer;
}

// recursive expression handling
char* generate_expr(ASTNode* node) {

    if (node->type == AST_NUMBER) {
        char* temp = new_temp();
        printf("%s = %d\n", temp, node->value);
        return temp;
    }

    if (node->type == AST_VARIABLE) {
        return node->name;
    }

    if (node->type == AST_BINARY) {
        char* left = generate_expr(node->left);
        char* right = generate_expr(node->right);

        char* temp = new_temp();

        printf("%s = %s + %s\n", temp, left, right); // your operator mapping later
        return temp;
    }

    return "";
}

// statement handling
void generate_stmt(ASTNode* node) {

    if (!node) return;

    switch(node->type) {

        case AST_ASSIGNMENT: {
            char* result = generate_expr(node->left);
            printf("%s = %s\n", node->name, result);
            break;
        }

        case AST_PRINT: {
            char* result = generate_expr(node->left);
            printf("print %s\n", result);
            break;
        }

        default:
            break;
    }
}

// main IR generator
void generate_ir(ASTNode* root) {

    ASTNode* current = root->left;

    while (current) {
        generate_stmt(current);
        current = current->next;
    }
}