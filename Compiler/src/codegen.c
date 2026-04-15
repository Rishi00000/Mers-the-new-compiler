#include <stdio.h>
#include "../include/codegen.h"
#include "../include/token.h"
#include "../include/ast.h"

int label_count = 0;

// generate labels like .L1, .L2...
char* gen_label() {
    static char buffer[20];
    sprintf(buffer, ".L%d", label_count++);
    return buffer;
}

void generate_code_expr(ASTNode* node) {

    if (node->type == AST_NUMBER) {
        printf("    mov rax, %d\n", node->value);
        return;
    }

    if (node->type == AST_VARIABLE) {
        printf("    mov rax, [%s]\n", node->name);
        return;
    }

    if (node->type == AST_BINARY) {

        // left → rax
        generate_code_expr(node->left);
        printf("    push rax\n");

        // right → rax
        generate_code_expr(node->right);
        printf("    mov rbx, rax\n");

        printf("    pop rax\n");

        // your language mapping: '-' means addition
        printf("    add rax, rbx\n");
    }

    if (node->type == AST_COMPARISON) {
        // left → rax
        generate_code_expr(node->left);
        printf("    push rax\n");

        // right → rax
        generate_code_expr(node->right);
        printf("    mov rbx, rax\n");

        printf("    pop rax\n");

        // Compare: rax cmp rbx
        printf("    cmp rax, rbx\n");

        // The result is stored in the flags register, not in rax
        // This is handled by the conditional jumps in the if statement
    }
}

void generate_code_stmt(ASTNode* node) {

    if (!node) return;

    switch(node->type) {

        case AST_ASSIGNMENT:
            generate_code_expr(node->left);
            printf("    mov [%s], rax\n", node->name);
            break;

        case AST_PRINT:
            generate_code_expr(node->left);
            printf("    ; print rax (not implemented fully)\n");
            break;

        case AST_IF: {
            char* else_label = gen_label();
            char* end_label = gen_label();

            // Generate comparison
            generate_code_expr(node->condition);

            // Conditional jump based on comparison operator
            const char* jump_instr = "je";  // default to equal

            if (node->condition->operator == TOKEN_LT) jump_instr = "jge";   // jump if NOT less than
            else if (node->condition->operator == TOKEN_GT) jump_instr = "jle";   // jump if NOT greater than
            else if (node->condition->operator == TOKEN_LE) jump_instr = "jg";    // jump if NOT less-equal
            else if (node->condition->operator == TOKEN_GE) jump_instr = "jl";    // jump if NOT greater-equal
            else if (node->condition->operator == TOKEN_EQ) jump_instr = "jne";   // jump if NOT equal
            else if (node->condition->operator == TOKEN_NE) jump_instr = "je";    // jump if NOT not-equal

            printf("    %s %s\n", jump_instr, else_label);

            // Generate true branch
            ASTNode* stmt = node->true_branch;
            while (stmt) {
                generate_code_stmt(stmt);
                stmt = stmt->next;
            }

            if (node->false_branch) {
                printf("    jmp %s\n", end_label);
                printf("%s:\n", else_label);

                // Generate false branch
                stmt = node->false_branch;
                while (stmt) {
                    generate_code_stmt(stmt);
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

void generate_code(ASTNode* root) {

    printf("\n=== Assembly ===\n");

    printf("section .data\n");

    // declare variables
    ASTNode* curr = root->left;
    while (curr) {
        if (curr->type == AST_DECLARATION) {
            printf("%s: dq 0\n", curr->name);
        }
        curr = curr->next;
    }

    printf("\nsection .text\n");
    printf("global _start\n\n");
    printf("_start:\n");

    curr = root->left;
    while (curr) {
        generate_code_stmt(curr);
        curr = curr->next;
    }

    printf("\n    mov rax, 60\n");
    printf("    mov rdi, 0\n");
    printf("    syscall\n");
}