#include <string.h>
#include "../include/symbol_table.h"

void init_table(SymbolTable *table) {
    table->count = 0;
}

int find_symbol(SymbolTable *table, const char *name) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].name, name) == 0)
            return 1;
    }
    return 0;
}

int add_symbol(SymbolTable *table, const char *name) {
    if (find_symbol(table, name))
        return 0; // duplicate

    strcpy(table->symbols[table->count].name, name);
    table->count++;
    return 1;
}