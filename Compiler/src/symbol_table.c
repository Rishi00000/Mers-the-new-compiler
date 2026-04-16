#include <stdio.h>
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

int add_symbol(SymbolTable *table, const char *name, const char *type) {
    if (find_symbol(table, name))
        return 0; // duplicate

    strcpy(table->symbols[table->count].name, name);
    strcpy(table->symbols[table->count].type, type);
    table->symbols[table->count].value = 0;
    table->symbols[table->count].is_initialized = 1;  // declared but not initialized with value
    table->count++;
    return 1;
}

int update_symbol_value(SymbolTable *table, const char *name, int value) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            table->symbols[i].value = value;
            table->symbols[i].is_initialized = 2;  // initialized with value
            return 1;
        }
    }
    return 0;  // symbol not found
}

void print_symbol_table(SymbolTable *table) {
    if (table->count == 0) {
        printf("  [Symbol Table] Empty\n");
        return;
    }
    
    printf("  [Symbol Table] Total Symbols: %d\n", table->count);
    printf("  %-20s %-15s %-20s\n", "Name", "Type", "Value");
    printf("  %-20s %-15s %-20s\n", "--------------------", "---------------", "--------------------");
    
    for (int i = 0; i < table->count; i++) {
        const char *value_str;
        if (table->symbols[i].is_initialized == 2) {
            char value_buffer[20];
            snprintf(value_buffer, sizeof(value_buffer), "%d", table->symbols[i].value);
            printf("  %-20s %-15s %-20s\n", 
                   table->symbols[i].name, 
                   table->symbols[i].type, 
                   value_buffer);
        } else {
            printf("  %-20s %-15s %-20s\n", 
                   table->symbols[i].name, 
                   table->symbols[i].type, 
                   "undeclared");
        }
    }
}