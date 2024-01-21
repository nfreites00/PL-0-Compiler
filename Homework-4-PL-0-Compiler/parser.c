// DISCLAIMER: we wrote parser.c using the TA Noelle's pseudocode for the parser.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"

#define MAX_CODE_LENGTH 500
#define MAX_SYMBOL_TABLE_SIZE 100
#define CONST_KIND 1
#define VAR_KIND 2
#define PROC_KIND 3

// Global variables to keep track of tokens; amounts of tokens and vars; and indexes.
int token, digit, symbol_index = 0, num_token = 0;

char identifier[11]; // Global array holding name of identifier.

symbol *parser(lexeme *list) {
	symbol *table = calloc(MAX_SYMBOL_TABLE_SIZE, sizeof(symbol));
  program_p(table, list); // Starts the program.
	return(table);
}

void program_p(symbol *table, lexeme *list) {
	get_token(list);
  block_p(0, table, list);
  if(token != periodsym)
    error(6);
  get_token(list);
  if(token != -1)
    error(16);
}

// This function checks to see if the token that is taken from the lexeme list is either a constant, variable or procedure.
void block_p(int lexlevel, symbol *table, lexeme *list) {
	int num_symbols = 0;

	// Constant declaration.
	if(token == constsym) {
    do {
      get_token(list);
      if(token != identsym)
        error(4);
			
			for (int i = 0; i <= symbol_index; i++) {
				if(strcmp(identifier, table[i].name) == 0) {
					if (table[i].mark == 0 && table[i].level == lexlevel)
						error(18);
				}
			}

      get_token(list);
      if(token != eqlsym) {
				if(token == becomessym)
					error(1);
				else
					error(3);
			}
			get_token(list);
      if(token != numbersym)
        error(2);

			symbol_table_add(1, lexlevel, 0, digit, 0, identifier, table);
			symbol_index++;
			num_symbols++;

      get_token(list);
    } while (token == commasym);

    if(token != semicolonsym)
      error(5);

    get_token(list);
	}
// Variable declaration.
	if(token == varsym) {
    int var_cnt = 0;
    do {
      get_token(list);
      if(token != identsym)
        error(4);
			for (int i = 0; i <= symbol_index; i++) {
				if(strcmp(identifier, table[i].name) == 0) {
					if (table[i].mark == 0 && table[i].level == lexlevel)
						error(18);
				}
			}

    	var_cnt++;
			symbol_table_add(2, lexlevel, var_cnt + 3, 0, 0, identifier, table); // Adds the vars to the symbol table.
			symbol_index++;
			num_symbols++;
			
      get_token(list);
    } while (token == commasym);

    if(token != semicolonsym)
      error(5);
    get_token(list);
	}
// Procedure declaration.
	if(token == procsym) {
    do {
      get_token(list);
      if(token != identsym)
        error(4);

			for (int i = 0; i <= symbol_index; i++) {
				if(strcmp(identifier, table[i].name) == 0) {
					if (table[i].mark == 0 && table[i].level == lexlevel)
						error(18);
				}
			}

			symbol_table_add(3, lexlevel, 0, 0, 0, identifier, table); // Adds the procedure to the symbol table.
			symbol_index++;
			num_symbols++;

      get_token(list);
      if(token != semicolonsym)
        error(5);
      get_token(list);
      block_p(lexlevel + 1, table, list);
      if(token != semicolonsym)
        error(5);
      get_token(list);
    } while(token == procsym);
	}

  statement_p(lexlevel, table, list);
	int count = 0;
	for (int i = symbol_index; i >= 0; i--) { // mark the last n number of unmarked symbols
		if (table[i].mark == 0) {
			table[i].mark = 1;
			count++;
			if (count == num_symbols) {
				break;
			}
		}
	}
}

// This function checks to see if the token taken from the lexeme list is either an identsym, a callsym, a beginsym, an ifsym, a while, read, or write.
void statement_p(int lexlevel, symbol *table, lexeme *list) {
  if(token == identsym) {
    for (int i = 0; i <= symbol_index; i++) {
			if((strcmp(identifier, table[i].name) == 0) && (table[i].mark == 0)) {
				if(table[i].kind != VAR_KIND) {
					error(22);
				}
				break;
			} else if((strcmp(identifier, table[i].name) != 0) && (i == symbol_index)) {
				error(7);
				}
		}

    get_token(list);
    if(token != becomessym)
      error(9);
    get_token(list);
    expression_p(lexlevel, table, list);
    return;
  }

  if(token == callsym) {
    get_token(list);
    if(token != identsym)
      error(4);

    for (int i = 0; i <= symbol_index; i++) {
			if((strcmp(identifier, table[i].name) == 0) && (table[i].mark == 0)) {
				if(table[i].kind != PROC_KIND) {
					error(23);
				}
				break;
			} else if((strcmp(identifier, table[i].name) != 0) && (i == symbol_index)) {
				error(7);
				}
		}
    
    get_token(list);
    return;
  }

  if(token == beginsym) {
    get_token(list);
    statement_p(lexlevel, table, list);
    while (token == semicolonsym) {
      get_token(list);
      statement_p(lexlevel, table, list);
    }
    if(token != endsym)
      error(11);
    get_token(list);
    return;
  }

  if(token == ifsym) {
    get_token(list);
    condition_p(lexlevel, table, list);
    if(token != thensym) {
      error(10);
		}
    get_token(list);
    statement_p(lexlevel, table, list);
    if(token == elsesym) {
			get_token(list);
      statement_p(lexlevel, table, list);
		}
    return;
  }

  if(token == whilesym) {
    get_token(list);
    condition_p(lexlevel, table, list);
    if(token != dosym)
      error(12);
    get_token(list);
    statement_p(lexlevel, table, list);
    return;
  }

  if(token == readsym) {
    get_token(list);
    if(token != identsym)
      error(4);

    for (int i = 0; i <= symbol_index; i++) {
			if(strcmp(identifier, table[i].name) == 0) {
				if(table[i].kind != VAR_KIND)
					error(22);
				break;
			} else if((strcmp(identifier, table[i].name) != 0) && (i == symbol_index)) {
				error(7);
				}
		}
		
    get_token(list);
    return;
  }

  if(token == writesym) {
    get_token(list);
    expression_p(lexlevel, table, list);
  }
}

// This function checks to see if the token taken from the lexeme list is either an oddsym, or if it's one of the relation operators.
void condition_p(int lexlevel, symbol *table, lexeme *list) {
  if(token == oddsym) {
    get_token(list);
    expression_p(lexlevel, table, list);
  }

  else {
    expression_p(lexlevel, table, list);
    int relOp = relation(token); // Saves the relation operator.
    if (relOp == 0)
      error(13);
    get_token(list);
    expression_p(lexlevel, table, list);
  }
}

// This function checks to see if the token taken from the lexeme list is either a plus or minus sign.
void expression_p(int lexlevel, symbol *table, lexeme *list) {
  if(token == plussym || token == minussym) {
    get_token(list);
	}

  term_p(lexlevel, table, list);

  while(token == plussym || token == minussym) {
    get_token(list);
    term_p(lexlevel, table, list);
  }
}

// This function checks to see if the token taken from the lexeme list is either a multiplication or division sign.
void term_p(int lexlevel, symbol *table, lexeme *list) {
  factor_p(lexlevel, table, list);
  while(token == multsym || token == slashsym) {
    get_token(list);
    factor_p(lexlevel, table, list);
  }
}

// This function checks to see if the token taken from the lexeme list is either an identsym, number, or a pair of parenthesis.
void factor_p(int lexlevel, symbol *table, lexeme *list) {
  if(token == identsym) {
    for (int i = 0; i <= symbol_index; i++) {
			if((strcmp(identifier, table[i].name) == 0) && (table[i].mark == 0)) {
				if(table[i].kind == PROC_KIND) {
					error(17);
				}
				break;
			} else if((strcmp(identifier, table[i].name) != 0) && (i == symbol_index)) {
				error(7);
				}
		}
    get_token(list);
  }

  else if(token == numbersym) {
    get_token(list);
  }

  else if(token == lparentsym) {
    get_token(list);
    expression_p(lexlevel, table, list);
    if(token != rparentsym) {
      error(14);
		}
    get_token(list);
  }

  else
    error(15);
}

// Switch statement inside this function returns the different relation operators.
int relation(int token) {
  switch(token) {
    case eqlsym:
      return eql;
    case neqsym:
      return neq;
    case lessym:
      return lss;
    case leqsym:
      return leq;
    case gtrsym:
      return gtr;
    case geqsym:         
      return geq;
    default:
      return 0;
  }
}

// This function gets the next token from the lexeme list.
void get_token(lexeme *list) {
	token = list[num_token].type;
	if(token == identsym) {
		int index = 0;
		while(list[num_token].name[index] != '\0') {
			identifier[index] = list[num_token].name[index];
			index++;
		}
		identifier[index] = '\0';
	}

	if(token == numbersym)
		digit = list[num_token].value;

	num_token++;
}

// searches for the symbol inside the symbol table.
int searchSymbol(char name[11], symbol *table) {
  int i = 0;
  while(i <= symbol_index) { // Return if symbol is in symbol table.
		if((strcmp(name, table[i].name) == 0) && (table[i].mark == 0))
    	return i;
		i++;
	}
	return -1;
}

// Adds the symbol's kind, val, level, addr, and mark into the symbol table.
void symbol_table_add(int kind, int l, int m, int val, int mark, char name[11], symbol *table) {
	table[symbol_index].kind = kind;
	table[symbol_index].val = val;
	table[symbol_index].level = l;
	table[symbol_index].addr = m;
	table[symbol_index].mark = mark;

	int index = 0;
	while(identifier[index] != '\0') {
		table[symbol_index].name[index] = identifier[index];
		index++;
	}
}

// List of errors throughout the code.
void error(int errors) {
  switch(errors) {
    case 1:
      printf("Error 1: Use = instead of :=.\n");
      break;
    case 2:
      printf("Error 2: = must be followed by a number.\n");
      break;
    case 3:
      printf("Error 3: Identifier must be followed by =.\n");
      break;
    case 4:
      printf("Error 4: const, var must be followed by an identifier.\n");
      break;
    case 5:
      printf("Error 5: Semicolon or comma missing.\n");
      break;
    case 6:
      printf("Error 6: Period expected.\n");
      break;
    case 7:
      printf("Error 7: Undeclared identifier.\n");
      break;
    case 8:
      printf("Error 8: Assignment to constant is not allowed.\n");
      break;
    case 9:
      printf("Error 9: Assignment operator expected.\n");
      break;
    case 10:
      printf("Error 10: then expected.\n");
      break;
    case 11:
      printf("Error 11: Semicolon or end expected.\n");
      break;
    case 12:
      printf("Error 12: do expected.\n");
      break;
    case 13:
      printf("Error 13: Relational operator expected.\n");
      break;
    case 14:
      printf("Error 14: Right parenthesis missing.\n");
      break;
    case 15:
      printf("Error 15: The preceding factor cannot begin with this symbol.\n");
      break;
		case 16:
			printf("Error 16: 8dentifier has to be a const or var in factor().\n");
			break;
    case 17:
			printf("Error 17: Identifier is already in the symbol table unmarked and at the same level.\n");
      break;
    case 18:
      printf("Error 18: Symbol table kind should be a variable.\n");
      break;
    case 22:
      printf("Error 22: Symbol table kind should be a procedure.\n");
      break;
    default:
      break;
  }
  exit(0); // Stops program when error occurs.
}