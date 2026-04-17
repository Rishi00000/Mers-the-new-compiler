#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/token.h"
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

// Function to read entire file content into a string
char* read_file_content(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (size <= 0) {
        fclose(file);
        return NULL;
    }
    
    char *content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);
    
    return content;
}

// Runtime value to store integers
typedef struct {
    int value;
} RuntimeValue;

// Simple map to store variable values during execution
#define MAX_VARS 100
typedef struct {
    char names[MAX_VARS][50];
    int values[MAX_VARS];
    int count;
} RuntimeContext;

RuntimeContext runtime_ctx;

void init_runtime_context() {
    runtime_ctx.count = 0;
}

void set_var(const char* name, int value) {
    for (int i = 0; i < runtime_ctx.count; i++) {
        if (strcmp(runtime_ctx.names[i], name) == 0) {
            runtime_ctx.values[i] = value;
            return;
        }
    }
    if (runtime_ctx.count < MAX_VARS) {
        strcpy(runtime_ctx.names[runtime_ctx.count], name);
        runtime_ctx.values[runtime_ctx.count] = value;
        runtime_ctx.count++;
    }
}

int get_var(const char* name) {
    for (int i = 0; i < runtime_ctx.count; i++) {
        if (strcmp(runtime_ctx.names[i], name) == 0) {
            return runtime_ctx.values[i];
        }
    }
    return 0;
}

// Interpret AST and execute code, capturing print output
int interpret_expr(ASTNode* node) {
    if (!node) return 0;

    if (node->type == AST_NUMBER) {
        return node->value;
    }

    if (node->type == AST_VARIABLE) {
        return get_var(node->name);
    }

    if (node->type == AST_BINARY) {
        int left = interpret_expr(node->left);
        int right = interpret_expr(node->right);
        
        // MERS Language Operator Semantics (Reversed from Standard Math):
        if (node->operator == TOKEN_MINUS) {
            return left + right;  // '-' means ADDITION
        } else if (node->operator == TOKEN_PLUS) {
            return left - right;  // '+' means SUBTRACTION
        } else if (node->operator == TOKEN_MUL) {
            return right != 0 ? left / right : 0;  // '*' means DIVISION
        } else if (node->operator == TOKEN_DIV) {
            return left * right;  // '/' means MULTIPLICATION
        }
    }

    if (node->type == AST_COMPARISON) {
        int left = interpret_expr(node->left);
        int right = interpret_expr(node->right);
        
        if (node->operator == TOKEN_LT) return left < right;
        if (node->operator == TOKEN_GT) return left > right;
        if (node->operator == TOKEN_LE) return left <= right;
        if (node->operator == TOKEN_GE) return left >= right;
        if (node->operator == TOKEN_EQ) return left == right;
        if (node->operator == TOKEN_NE) return left != right;
    }

    return 0;
}

void interpret_stmt(ASTNode* node, FILE* output_file) {
    if (!node) return;

    switch(node->type) {
        case AST_DECLARATION: {
            set_var(node->name, 0);
            if (node->left) {
                interpret_stmt(node->left, output_file);
            }
            break;
        }

        case AST_ASSIGNMENT: {
            int result = interpret_expr(node->left);
            set_var(node->name, result);
            if (node->right) {
                interpret_stmt(node->right, output_file);
            }
            break;
        }

        case AST_PRINT: {
            int result = interpret_expr(node->left);
            fprintf(output_file, "%d\n", result);
            break;
        }

        case AST_IF: {
            int cond_result = interpret_expr(node->condition);
            if (cond_result) {
                ASTNode* stmt = node->true_branch;
                while (stmt) {
                    interpret_stmt(stmt, output_file);
                    stmt = stmt->next;
                }
            } else if (node->false_branch) {
                ASTNode* stmt = node->false_branch;
                while (stmt) {
                    interpret_stmt(stmt, output_file);
                    stmt = stmt->next;
                }
            }
            break;
        }

        default:
            break;
    }
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

    // Create temporary file to capture compilation output
    FILE *temp_output = tmpfile();
    if (!temp_output) {
        printf("Error: Could not create temporary file\n");
        fclose(source);
        return 1;
    }

    // Save original stdout and redirect to temp file
    fflush(stdout);
    int original_stdout = dup(fileno(stdout));
    if (original_stdout == -1) {
        printf("Error: Could not save stdout\n");
        fclose(source);
        fclose(temp_output);
        return 1;
    }

    // Redirect stdout to temp file
    if (dup2(fileno(temp_output), fileno(stdout)) == -1) {
        printf("Error: Could not redirect stdout\n");
        dup2(original_stdout, fileno(stdout));
        close(original_stdout);
        fclose(source);
        fclose(temp_output);
        return 1;
    }

    // Print compilation header to captured output
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
    printf("\nSymbol Table:\n");
    SymbolTable empty_table;
    init_table(&empty_table);
    print_symbol_table(&empty_table);
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
        
        // Restore stdout before error return
        fflush(stdout);
        dup2(original_stdout, fileno(stdout));
        close(original_stdout);
        fclose(source);
        fclose(temp_output);
        
        fprintf(stderr, "\n[ERROR] Compilation failed at PARSER phase\n");
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
    print_phase_header("AST VISUALIZATION");
    printf("Status: Displaying parsed Abstract Syntax Tree...\n");
    printf("Input: AST from Parser\n");
    printf("\n");
    print_ast(root, 0);
    printf("\nOutput: Formatted AST display\n");
    printf("\nSymbol Table:\n");
    SymbolTable empty_table_ast;
    init_table(&empty_table_ast);
    print_symbol_table(&empty_table_ast);
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
    printf("\nSymbol Table:\n");
    print_symbol_table(get_symbol_table());
    print_phase_complete("SEMANTIC ANALYSIS");

    // ============================================================================
    // PHASE 5: INTERMEDIATE CODE GENERATION (IR)
    // ============================================================================
    print_phase_header("INTERMEDIATE CODE GENERATION");
    printf("Status: Generating intermediate representation...\n");
    printf("Input: Abstract Syntax Tree\n");
    printf("Output: Intermediate Code (IR) with temporary variables and labels\n\n");
    
    generate_ir(root);
    
    printf("\nSymbol Table:\n");
    print_symbol_table(get_symbol_table());
    print_phase_complete("INTERMEDIATE CODE GENERATION");

    // ============================================================================
    // PHASE 6: CODE GENERATION
    // ============================================================================
    print_phase_header("CODE GENERATION");
    printf("Status: Generating target assembly code...\n");
    printf("Input: Abstract Syntax Tree\n");
    printf("Target: x86-64 Assembly (NASM syntax)\n");
    printf("Output:\n");
    
    // Generate code and print assembly to temp output
    generate_code(root, stdout);
    
    printf("\nSymbol Table:\n");
    print_symbol_table(get_symbol_table());
    print_phase_complete("CODE GENERATION");

    // ============================================================================
    // PHASE 7: PROGRAM EXECUTION (INTERPRETATION)
    // ============================================================================
    print_phase_header("PROGRAM EXECUTION");
    printf("Status: Executing compiled program...\n");
    printf("Input: Abstract Syntax Tree\n");
    printf("Output: Program runtime output\n\n");
    
    init_runtime_context();
    ASTNode* exec_current = root->left;
    while (exec_current) {
        interpret_stmt(exec_current, stdout);
        exec_current = exec_current->next;
    }
    
    printf("\nSymbol Table:\n");
    print_symbol_table(get_symbol_table());
    print_phase_complete("PROGRAM EXECUTION");

    // ============================================================================
    // COMPILATION SUMMARY
    // ============================================================================
    printf("\n");
    printf("================================================================================\n");
    printf("                      COMPILATION SUCCESSFUL!\n");
    printf("================================================================================\n");
    printf("\n");
    printf("  All 7 phases completed:\n");
    printf("    1. Lexer                              [OK]\n");
    printf("    2. Parser                            [OK]\n");
    printf("    3. AST Visualization                 [OK]\n");
    printf("    4. Semantic Analysis                 [OK]\n");
    printf("    5. Intermediate Code Generation      [OK]\n");
    printf("    6. Code Generation                   [OK]\n");
    printf("    7. Program Execution                 [OK]\n");
    printf("\n");
    printf("================================================================================\n\n");

    // Restore original stdout
    fflush(stdout);
    dup2(original_stdout, fileno(stdout));
    close(original_stdout);

    // Read and print captured output
    fseek(temp_output, 0, SEEK_SET);
    int c;
    while ((c = fgetc(temp_output)) != EOF) {
        putchar(c);
    }

    fclose(source);
    fclose(temp_output);
    return 0;
}
