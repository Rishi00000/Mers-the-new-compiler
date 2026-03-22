#include <stdio.h>
#include "../include/ir.h"
#include "../include/token.h"

int temp_count = 0;
int ir_label_count = 0;

// generate temporary variables like t1, t2...
char* new_temp() {
    static char buffer[20];
    sprintf(buffer, "t%d", temp_count++);
    return buffer;
}

// generate labels like L1, L2...
char* new_label() {
    static char buffer[20];
    sprintf(buffer, "L%d", ir_label_count++);
    return buffer;
}

// recursive expression handling
char* generate_ir_expr(ASTNode* node) {

    if (node->type == AST_NUMBER) {
        char* temp = new_temp();
        printf("%s = %d\n", temp, node->value);
        return temp;
    }

    if (node->type == AST_VARIABLE) {
        return node->name;
    }

    if (node->type == AST_BINARY) {
        char* left = generate_ir_expr(node->left);
        char* right = generate_ir_expr(node->right);

        char* temp = new_temp();

        printf("%s = %s + %s\n", temp, left, right); // your operator mapping later
        return temp;
    }

    if (node->type == AST_COMPARISON) {
        char* left = generate_ir_expr(node->left);
        char* right = generate_ir_expr(node->right);

        char* temp = new_temp();
        const char* cmp_op = "==";

        if (node->operator == TOKEN_LT) cmp_op = "<";
        else if (node->operator == TOKEN_GT) cmp_op = ">";
        else if (node->operator == TOKEN_LE) cmp_op = "<=";
        else if (node->operator == TOKEN_GE) cmp_op = ">=";
        else if (node->operator == TOKEN_EQ) cmp_op = "==";
        else if (node->operator == TOKEN_NE) cmp_op = "!=";

        printf("%s = %s %s %s\n", temp, left, cmp_op, right);
        return temp;
    }

    return "";
}

// statement handling
void generate_ir_stmt(ASTNode* node) {

    if (!node) return;

    switch(node->type) {

        case AST_ASSIGNMENT: {
            char* result = generate_ir_expr(node->left);
            printf("%s = %s\n", node->name, result);
            break;
        }

        case AST_PRINT: {
            char* result = generate_ir_expr(node->left);
            printf("print %s\n", result);
            break;
        }

        case AST_IF: {
            char* else_label = new_label();
            char* end_label = new_label();

            char* cond_result = generate_ir_expr(node->condition);
            printf("if_not %s goto %s\n", cond_result, else_label);

            // Generate true branch
            ASTNode* stmt = node->true_branch;
            while (stmt) {
                generate_ir_stmt(stmt);
                stmt = stmt->next;
            }

            if (node->false_branch) {
                printf("goto %s\n", end_label);
                printf("%s:\n", else_label);

                // Generate false branch
                stmt = node->false_branch;
                while (stmt) {
                    generate_ir_stmt(stmt);
                    stmt = stmt->next;
                }

                printf("%s:\n", end_label);
            } else {
                printf("%s:\n", else_label);
            }

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
        generate_ir_stmt(current);
        current = current->next;
    }
}