#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_SYMBOLS 100

typedef struct {
    char name[64];
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
} SymbolTable;

void init_table(SymbolTable *table);
int add_symbol(SymbolTable *table, const char *name);
int find_symbol(SymbolTable *table, const char *name);

#endif