#ifndef __CODEGEN_H
#define __CODEGEN_H

#include "lex.h"
#include "parser.h"

// Holds values for the generated code array.
typedef struct instruction {
  int opcode;
  char *op;
  int l;
  int m;
} instruction;

// Enum to keep track of each ISA operation.
typedef enum {
	lit = 1, opr, lod, sto, cal, inc, jmp, jpc, sio1, sio2, sio3
} isa_ops;

instruction *codegen(symbol *table, lexeme *list, int aFlag);
void program_cg(int lexlevel, instruction *code, lexeme *list, symbol *table);
void block_cg(int procIndex, int lexlevel, instruction *code, lexeme *list, symbol *table);
void statement_cg(int lexlevel, instruction *code, lexeme *list, symbol *table);
void condition_cg(int lexlevel, instruction *code, lexeme *list, symbol *table);
int relation_cg(int token);
void expression_cg(int lexlevel, instruction *code, lexeme *list, symbol *table);
void term_cg(int lexlevel, instruction *code, lexeme *list, symbol *table);
void factor_cg(int lexlevel, instruction *code, lexeme *list, symbol *table);
void emit(int opcode, int l, int m, char *op, instruction* code);
void get_token_cg(lexeme *list);
void print_assembly(instruction *code);
void print_symbol_table_cg(symbol *table);

#endif