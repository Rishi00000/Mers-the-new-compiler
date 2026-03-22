#include <stdio.h>
#include "../include/parser.h"
#include "../include/ast.h"
#include "../include/semantic.h"
#include "../include/ir.h"
#include "../include/codegen.h"

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
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *source = fopen(argv[1], "r");

    if (!source) {
        printf("Error opening file\n");
        return 1;
    }

    printf("\n");
    printf("================================================================================\n");
    printf("                    MERS COMPILER - COMPILATION PROCESS\n");
    printf("================================================================================\n");

    // ============================================================================
    // PHASE 1: LEXER
    // ============================================================================
    print_phase_header("LEXER");
    printf("Status: Reading and tokenizing source file...\n");
    printf("Input: Lexical analysis of source code\n");
    printf("Output: Stream of tokens\n");
    print_phase_complete("LEXER");

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
    print_phase_complete("PARSER");

    // ============================================================================
    // PHASE 3: AST VISUALIZATION
    // ============================================================================
    print_phase_header("AST VISUALIZATION");
    printf("Status: Displaying parsed Abstract Syntax Tree...\n");
    printf("Input: AST from Parser\n");
    printf("\n");
    print_ast(root, 0);
    printf("\nOutput: Formatted AST display\n");
    print_phase_complete("AST VISUALIZATION");

    // ============================================================================
    // PHASE 4: SEMANTIC ANALYSIS
    // ============================================================================
    print_phase_header("SEMANTIC ANALYSIS");
    printf("Status: Validating semantic correctness...\n");
    printf("Input: Abstract Syntax Tree\n");
    printf("Checks: Variable declarations, scope, type consistency\n\n");
    
    analyze(root);
    
    printf("\nOutput: Validation complete (no errors means semantic correctness)\n");
    print_phase_complete("SEMANTIC ANALYSIS");

    // ============================================================================
    // PHASE 5: INTERMEDIATE CODE GENERATION (IR)
    // ============================================================================
    print_phase_header("INTERMEDIATE CODE GENERATION");
    printf("Status: Generating intermediate representation...\n");
    printf("Input: Abstract Syntax Tree\n");
    printf("Output: Intermediate Code (IR) with temporary variables and labels\n\n");
    
    generate_ir(root);
    
    print_phase_complete("INTERMEDIATE CODE GENERATION");

    // ============================================================================
    // PHASE 6: CODE GENERATION
    // ============================================================================
    print_phase_header("CODE GENERATION");
    printf("Status: Generating target assembly code...\n");
    printf("Input: Abstract Syntax Tree\n");
    printf("Target: x86-64 Assembly (NASM syntax)\n");
    printf("Output:\n");
    
    generate_code(root);
    
    print_phase_complete("CODE GENERATION");

    // ============================================================================
    // COMPILATION SUMMARY
    // ============================================================================
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

    fclose(source);
    return 0;
}
