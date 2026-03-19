#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "ast.h"

ASTNode* parse(FILE *source);

#endif