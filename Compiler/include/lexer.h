#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"

Token get_next_token(FILE *source);

#endif