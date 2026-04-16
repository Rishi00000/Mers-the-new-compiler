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

void generate_code_expr(ASTNode* node, FILE* output) {

    if (node->type == AST_NUMBER) {
        fprintf(output, "    mov rax, %d\n", node->value);
        return;
    }

    if (node->type == AST_VARIABLE) {
        fprintf(output, "    mov rax, [%s]\n", node->name);
        return;
    }

    if (node->type == AST_BINARY) {

        // left → rax
        generate_code_expr(node->left, output);
        fprintf(output, "    push rax\n");

        // right → rax
        generate_code_expr(node->right, output);
        fprintf(output, "    mov rbx, rax\n");

        fprintf(output, "    pop rax\n");

        // your language mapping: '-' means addition
        fprintf(output, "    add rax, rbx\n");
    }

    if (node->type == AST_COMPARISON) {
        // left → rax
        generate_code_expr(node->left, output);
        fprintf(output, "    push rax\n");

        // right → rax
        generate_code_expr(node->right, output);
        fprintf(output, "    mov rbx, rax\n");

        fprintf(output, "    pop rax\n");

        // Compare: rax cmp rbx
        fprintf(output, "    cmp rax, rbx\n");

        // The result is stored in the flags register, not in rax
        // This is handled by the conditional jumps in the if statement
    }
}

void generate_code_stmt(ASTNode* node, FILE* output) {

    if (!node) return;

    switch(node->type) {

        case AST_ASSIGNMENT:
            generate_code_expr(node->left, output);
            fprintf(output, "    mov [%s], rax\n", node->name);
            break;

        case AST_PRINT:
            generate_code_expr(node->left, output);
            fprintf(output, "    ; print rax (not implemented fully)\n");
            break;

        case AST_IF: {
            char* else_label = gen_label();
            char* end_label = gen_label();

            // Generate comparison
            generate_code_expr(node->condition, output);

            // Conditional jump based on comparison operator
            const char* jump_instr = "je";  // default to equal

            if (node->condition->operator == TOKEN_LT) jump_instr = "jge";   // jump if NOT less than
            else if (node->condition->operator == TOKEN_GT) jump_instr = "jle";   // jump if NOT greater than
            else if (node->condition->operator == TOKEN_LE) jump_instr = "jg";    // jump if NOT less-equal
            else if (node->condition->operator == TOKEN_GE) jump_instr = "jl";    // jump if NOT greater-equal
            else if (node->condition->operator == TOKEN_EQ) jump_instr = "jne";   // jump if NOT equal
            else if (node->condition->operator == TOKEN_NE) jump_instr = "je";    // jump if NOT not-equal

            fprintf(output, "    %s %s\n", jump_instr, else_label);

            // Generate true branch
            ASTNode* stmt = node->true_branch;
            while (stmt) {
                generate_code_stmt(stmt, output);
                stmt = stmt->next;
            }

            if (node->false_branch) {
                fprintf(output, "    jmp %s\n", end_label);
                fprintf(output, "%s:\n", else_label);

                // Generate false branch
                stmt = node->false_branch;
                while (stmt) {
                    generate_code_stmt(stmt, output);
                    stmt = stmt->next;
                }

                fprintf(output, "%s:\n", end_label);
            } else {
                fprintf(output, "%s:\n", else_label);
            }

            break;
        }

        default:
            break;
    }
}

void generate_code(ASTNode* root, FILE* output) {

    fprintf(output, "\n=== Assembly ===\n");

    fprintf(output, "section .data\n");

    // declare variables
    ASTNode* curr = root->left;
    while (curr) {
        if (curr->type == AST_DECLARATION) {
            fprintf(output, "%s: dq 0\n", curr->name);
        }
        curr = curr->next;
    }

    fprintf(output, "\nsection .text\n");
    fprintf(output, "global _start\n\n");
    fprintf(output, "_start:\n");

    curr = root->left;
    while (curr) {
        generate_code_stmt(curr, output);
        curr = curr->next;
    }

    fprintf(output, "\n    mov rax, 60\n");
    fprintf(output, "    mov rdi, 0\n");
    fprintf(output, "    syscall\n");
}