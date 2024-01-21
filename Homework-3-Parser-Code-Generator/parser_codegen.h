#ifndef __PARSER_H
#define __PARSER_H

#include "lex.h"

// Struct for the symbol table.
typedef struct symbol { 
	int kind; 		// const = 1, var = 2
	char name[10];	// name up to 11 chars
	int val; 		// number (ASCII value) 
	int level; 		// L level
	int addr; 		// M address
	int mark;		// to indicate unavailable or deleted
} symbol;

// Holds values for the generated code array.
typedef struct instruction {
  int opcode;
  char *op;
  int l;
  int m;
} instruction;

// Enum to keep track of values for the opr operation.
typedef enum {
  ret = 0, neg, add, sub, mul, division, odd, mod, eql, neq, lss, leq, gtr, geq
} opr_functions;

// Enum to keep track of each ISA operation.
typedef enum {
	lit = 1, opr, lod, sto, temp, inc, jmp, jpc, sio1, sio2, sio3
} isa_ops;

instruction *parse_codegen(lexeme *list, int aFlag);
void program(lexeme *list, instruction *code);
void block(lexeme *list, instruction *code);
void statement(lexeme *list, instruction *code);
void condition(lexeme *list, instruction *code);
int relation(int token);
void expression(lexeme *list, instruction *code);
void term(lexeme *list, instruction *code);
void factor(lexeme *list, instruction *code);
void emit(int opcode, int l, int m, char *op, instruction* code);
void getToken(lexeme *list);
void symbolTableAdd(int kind, int l, int m, int val, int mark);
void setName();
int searchSymbol(char name[11]);
void error(int errors);
void printAssembly(instruction *code);

#endif