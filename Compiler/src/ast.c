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

    return node;
}