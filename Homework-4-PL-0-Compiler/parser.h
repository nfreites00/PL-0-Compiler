#ifndef __PARSER_H
#define __PARSER_H

#include "lex.h"

// Struct for the symbol table.
typedef struct { 
	int kind; 		// const = 1, var = 2, procedure = 3
	char name[10];	// name up to 11 chars
	int val; 		// number (ASCII value) 
	int level; 		// L level
	int addr; 		// M address
	int mark;		// to indicate unavailable or deleted
} symbol;

// Enum to keep track of values for the opr operation.
typedef enum {
  ret = 0, neg, add, sub, mul, division, odd, mod, eql, neq, lss, leq, gtr, geq
} opr_functions;

symbol *parser(lexeme *list);
void program_p(symbol *table, lexeme *list);
void block_p(int lexlevel, symbol *table, lexeme *list);
void statement_p(int lexlevel, symbol *table, lexeme *list);
void condition_p(int lexlevel, symbol *table, lexeme *list);
int relation(int token);
void expression_p(int lexlevel, symbol *table, lexeme *list);
void term_p(int lexlevel, symbol *table, lexeme *list);
void factor_p(int lexlevel, symbol *table, lexeme *list);
void get_token(lexeme *list);
void symbol_table_add(int kind, int l, int m, int val, int mark, char name[11], symbol *table);
void error(int errors);

#endif