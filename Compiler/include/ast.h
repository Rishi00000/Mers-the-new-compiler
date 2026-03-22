#ifndef AST_H
#define AST_H

typedef enum {
    AST_PROGRAM,
    AST_DECLARATION,
    AST_ASSIGNMENT,
    AST_PRINT,
    AST_BINARY,
    AST_COMPARISON,    // NEW: for condition expressions
    AST_NUMBER,
    AST_VARIABLE,
    AST_IF             // NEW: for if-else statements
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;

    char name[64];
    int value;
    int operator;      // NEW: for storing comparison operator (TOKEN_LT, TOKEN_GT, etc.)

    struct ASTNode *left;
    struct ASTNode *right;

    // NEW: for if-else statements
    struct ASTNode *condition;
    struct ASTNode *true_branch;
    struct ASTNode *false_branch;

    // for statement list
    struct ASTNode *next;

} ASTNode;

ASTNode* create_node(ASTNodeType type);
void print_ast(ASTNode* node, int level);

#endif