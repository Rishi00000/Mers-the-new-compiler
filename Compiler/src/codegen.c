#include <stdio.h>
#include "../include/codegen.h"

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