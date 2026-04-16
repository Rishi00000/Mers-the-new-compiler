#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_SYMBOLS 100

typedef struct {
    char name[64];
    char type[32];  // "int", "float", etc.
    int value;
    int is_initialized;  // 0 = undeclared, 1 = declared but uninitialized, 2 = initialized
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
} SymbolTable;

void init_table(SymbolTable *table);
int add_symbol(SymbolTable *table, const char *name, const char *type);
int find_symbol(SymbolTable *table, const char *name);
int update_symbol_value(SymbolTable *table, const char *name, int value);
void print_symbol_table(SymbolTable *table);

#endif