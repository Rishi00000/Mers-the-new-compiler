#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol_table.h"

void analyze(ASTNode* root);
SymbolTable* get_symbol_table(void);

#endif