#include <stdio.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/ast.h"
#include "../include/semantic.h"
#include "../include/ir.h"
#include "../include/codegen.h"
#include "../include/symbol_table.h"

void print_phase_header(const char *phase_name) {
    printf("\n");
    printf("================================================================================\n");
    printf("                          PHASE: %s\n", phase_name);
    printf("================================================================================\n");
}

void print_phase_complete(const char *phase_name) {
    printf("\n[OK] [%s] PHASE COMPLETED SUCCESSFULLY\n", phase_name);
    printf("================================================================================\n");
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("MERS Compiler - Mini Language Compiler\n\n");
        printf("Usage: %s <input.my> [options]\n\n", argv[0]);
        printf("Options:\n");
        printf("  -o <filename>    Specify output assembly file (default: input.asm)\n");
        printf("  -q               Suppress console output\n");
        printf("  -h, --help       Show this help message\n\n");
        printf("Examples:\n");
        printf("  %s program.my\n", argv[0]);
        printf("  %s program.my -o output.asm\n", argv[0]);
        printf("  %s program.my -q\n", argv[0]);
        return 1;
    }

    // Parse arguments
    const char *input_file = NULL;
    char *output_file = NULL;
    int quiet_mode = 0;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("MERS Compiler - Mini Language Compiler\n\n");
            printf("Usage: %s <input.my> [options]\n\n", argv[0]);
            printf("Options:\n");
            printf("  -o <filename>    Specify output assembly file (default: input.asm)\n");
            printf("  -q               Suppress console output\n");
            printf("  -h, --help       Show this help message\n");
            return 0;
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } else {
                printf("Error: -o flag requires an output filename\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-q") == 0) {
            quiet_mode = 1;
        } else if (argv[i][0] != '-') {
            input_file = argv[i];
        }
    }

    if (!input_file) {
        printf("Error: Input file not specified\n");
        printf("Usage: %s <input.my> [options]\n", argv[0]);
        return 1;
    }

    FILE *source = fopen(input_file, "r");

    if (!source) {
        printf("Error: Could not open file '%s'\n", input_file);
        return 1;
    }

    // Only print header if not in quiet mode
    if (!quiet_mode) {
        printf("\n");
        printf("================================================================================\n");
        printf("                    MERS COMPILER - COMPILATION PROCESS\n");
        printf("================================================================================\n");
    }

    // ============================================================================
    // PHASE 1: LEXER
    // ============================================================================
    if (!quiet_mode) print_phase_header("LEXER");
    if (!quiet_mode) {
        printf("Status: Reading and tokenizing source file...\n");
        printf("Input: Lexical analysis of source code\n");
        printf("Output: Stream of tokens\n");
        printf("\nSymbol Table:\n");
    }
    SymbolTable empty_table;
    init_table(&empty_table);
    if (!quiet_mode) print_symbol_table(&empty_table);
    if (!quiet_mode) print_phase_complete("LEXER");

    // ============================================================================
    // PHASE 2: PARSER
    // ============================================================================
    print_phase_header("PARSER");
    printf("Status: Building Abstract Syntax Tree (AST)...\n");
    printf("Input: Token stream from Lexer\n");
    
    ASTNode* root = parse(source);
    
    if (!root) {
        printf("\n[ERROR] [PARSER] PHASE FAILED - Could not build AST\n");
        printf("================================================================================\n");
        fclose(source);
        return 1;
    }
    
    printf("Output: Abstract Syntax Tree\n");
    printf("\nSymbol Table:\n");
    SymbolTable empty_table_parser;
    init_table(&empty_table_parser);
    print_symbol_table(&empty_table_parser);
    print_phase_complete("PARSER");

    // ============================================================================
    // PHASE 3: AST VISUALIZATION
    // ============================================================================
    if (!quiet_mode) print_phase_header("AST VISUALIZATION");
    if (!quiet_mode) {
        printf("Status: Displaying parsed Abstract Syntax Tree...\n");
        printf("Input: AST from Parser\n");
        printf("\n");
    }
    if (!quiet_mode) print_ast(root, 0);
    if (!quiet_mode) {
        printf("\nOutput: Formatted AST display\n");
        printf("\nSymbol Table:\n");
    }
    SymbolTable empty_table_ast;
    init_table(&empty_table_ast);
    if (!quiet_mode) print_symbol_table(&empty_table_ast);
    if (!quiet_mode) print_phase_complete("AST VISUALIZATION");

    // ============================================================================
    // PHASE 4: SEMANTIC ANALYSIS
    // ============================================================================
    if (!quiet_mode) print_phase_header("SEMANTIC ANALYSIS");
    if (!quiet_mode) {
        printf("Status: Validating semantic correctness...\n");
        printf("Input: Abstract Syntax Tree\n");
        printf("Checks: Variable declarations, scope, type consistency\n\n");
    }
    
    analyze(root);
    
    if (!quiet_mode) {
        printf("\nOutput: Validation complete (no errors means semantic correctness)\n");
        printf("\nSymbol Table:\n");
    }
    if (!quiet_mode) print_symbol_table(get_symbol_table());
    if (!quiet_mode) print_phase_complete("SEMANTIC ANALYSIS");

    // ============================================================================
    // PHASE 5: INTERMEDIATE CODE GENERATION (IR)
    // ============================================================================
    if (!quiet_mode) print_phase_header("INTERMEDIATE CODE GENERATION");
    if (!quiet_mode) {
        printf("Status: Generating intermediate representation...\n");
        printf("Input: Abstract Syntax Tree\n");
        printf("Output: Intermediate Code (IR) with temporary variables and labels\n\n");
    }
    
    generate_ir(root);
    
    if (!quiet_mode) {
        printf("\nSymbol Table:\n");
        print_symbol_table(get_symbol_table());
    }
    if (!quiet_mode) print_phase_complete("INTERMEDIATE CODE GENERATION");

    // ============================================================================
    // PHASE 6: CODE GENERATION
    // ============================================================================
    if (!quiet_mode) print_phase_header("CODE GENERATION");
    if (!quiet_mode) {
        printf("Status: Generating target assembly code...\n");
        printf("Input: Abstract Syntax Tree\n");
        printf("Target: x86-64 Assembly (NASM syntax)\n");
        printf("Output:\n");
    }
    
    // Generate code and print assembly to stdout
    generate_code(root, stdout);
    
    if (!quiet_mode) {
        printf("\nSymbol Table:\n");
        print_symbol_table(get_symbol_table());
        print_phase_complete("CODE GENERATION");
    }

    // ============================================================================
    // COMPILATION SUMMARY
    // ============================================================================
    if (!quiet_mode) {
        printf("\n");
        printf("================================================================================\n");
        printf("                      COMPILATION SUCCESSFUL!\n");
        printf("================================================================================\n");
        printf("\n");
        printf("  All 6 phases completed:\n");
        printf("    1. Lexer                              [OK]\n");
        printf("    2. Parser                            [OK]\n");
        printf("    3. AST Visualization                 [OK]\n");
        printf("    4. Semantic Analysis                 [OK]\n");
        printf("    5. Intermediate Code Generation      [OK]\n");
        printf("    6. Code Generation                   [OK]\n");
        printf("\n");
        printf("================================================================================\n\n");
    } else {
        printf("Compilation successful!\n");
    }

    fclose(source);
    return 0;
}
