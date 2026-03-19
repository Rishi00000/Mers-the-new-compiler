#ifndef AST_H
#define AST_H

typedef enum {
    AST_PROGRAM,
    AST_DECLARATION,
    AST_ASSIGNMENT,
    AST_PRINT,
    AST_BINARY,
    AST_NUMBER,
    AST_VARIABLE
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;

    char name[64];     // variable name
    int value;         // for numbers

    struct ASTNode *left;
    struct ASTNode *right;

} ASTNode;

ASTNode* create_node(ASTNodeType type);

#endif