// DISCLAIMER: we wrote codegen.c using the TA Noelle's pseudocode for the code generator.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"
#include "codegen.h"

#define MAX_CODE_LENGTH 500
#define INITIAL_PROC_INDEX -100
#define CONST_KIND 1
#define VAR_KIND 2
#define PROC_KIND 3

// Global variables to keep track of token_cgs; amounts of token_cgs and vars; and indexes.
int token_cg, digit_cg, cx, var_temp, proc_cnt = 0, symbol_index_cg = 0, num_symbols = 0, num_token_cg = 0;

char identifier[11]; // Global array holding name of identifier

instruction *codegen(symbol *table, lexeme *list, int aFlag) {
	int lexlevel = 0;
	instruction *code = calloc(MAX_CODE_LENGTH, sizeof(instruction));
  program_cg(lexlevel, code, list, table); // Starts the program.
	if (aFlag == 1) // Flag to print the code.
		print_assembly(code);

  return code;
}

void program_cg(int lexlevel, instruction *code, lexeme *list, symbol *table) {
  int main_index = cx;
  int index = 0;
  emit(jmp, 0, 0, "jmp", code); // emit JMP M = 0 for main

  for (int i = 0; list[i].type != -1; i++) {
		if (list[i].type == procsym) {
			proc_cnt++; // Count up procedure.
			emit(jmp, 0, 0, "jmp", code); // emit JMP M = 0 for every procedure
		}
	}

	get_token_cg(list);
  block_cg(INITIAL_PROC_INDEX, 0, code, list, table);
	get_token_cg(list); 

  // fix all of your JMP using the M values in the symbol table
	index = 0;
	for (int j = symbol_index_cg - 1; j >= 0; j--) {
		if(table[j].kind == PROC_KIND) {
			code[index].m = table[j].addr;
			index++;
			if (index > proc_cnt) {
				break;
			}
		}
	}
  emit(sio3, 0, 3, "sio", code); // emit halt.
}

// This function checks to see if the token is either a constant,  variable, or procedure.
void block_cg(int procIndex, int lexlevel, instruction *code, lexeme *list, symbol *table) {
  if(token_cg == constsym) { // Constant declaration.
    do {
      get_token_cg(list);
      get_token_cg(list);
			table[symbol_index_cg].mark = 0; // unmark them.
			symbol_index_cg++;
			num_symbols++;
      get_token_cg(list);
      get_token_cg(list);
    } while (token_cg == commasym);
		get_token_cg(list);
  }

  if(token_cg == varsym) { // Variable declaration.
		int var_cnt = 0;
    do {
      get_token_cg(list);
    	var_cnt++;
			num_symbols++;
			table[symbol_index_cg].mark = 0; // unmark them.
			symbol_index_cg++;
      get_token_cg(list);
    } while (token_cg == commasym);
    get_token_cg(list);
		var_temp = var_cnt;
  }

  if(token_cg == procsym) { // Procedure declaration.
    do {
      get_token_cg(list);
			num_symbols++;
			table[symbol_index_cg].mark = 0; // unmark
			procIndex = symbol_index_cg;
			symbol_index_cg++;
      get_token_cg(list);
      get_token_cg(list);
      block_cg(procIndex, lexlevel + 1, code, list, table);
      get_token_cg(list);
			emit(opr, 0, 0, "opr", code); // return
    } while(token_cg == procsym);
	}


  table[procIndex].addr = cx; // update the symbol table entry for the procedure and set m = current code index

  emit(inc, table[symbol_index_cg].level, 4 + var_temp, "inc", code); // emit (increase using the number of variable).
  statement_cg(lexlevel, code, list, table);

  int count = 0;
	for (int i = symbol_index_cg; i >= 0; i--) { // mark the last n number of unmarked symbols
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
void statement_cg(int lexlevel, instruction *code, lexeme *list, symbol *table) {
  if(token_cg == identsym) {
		int i = symbol_index_cg - 1, index = 0;
    // save the symbol table index of the entry unmarked and closest in lexlevel.
    while(i >= 0) {
			if(strcmp(identifier, table[i].name) == 0) {
        if (table[i].mark == 0 && table[i].level <= lexlevel) {
					index = i;
					break;
				}
			}
			i--;
		}
    get_token_cg(list);
    get_token_cg(list);
    expression_cg(lexlevel, code, list, table);
    // Emit store, L & M from the symbol table.
		emit(sto, lexlevel - table[index].level, table[index].addr, "sto", code);
  }

  if(token_cg == callsym) {
    get_token_cg(list);
    int i = symbol_index_cg - 1, index = 0;
    // Find the entry for procedure unmarked and closest in lexlevel.
    while(i >= 0) {
			if(strcmp(identifier, table[i].name) == 0) {
        if ((table[i].mark == 0) && (table[i].level) <= lexlevel && (table[i].kind == PROC_KIND)) {
					index = i;
					break;
				}
			}
			i--;
		}

    get_token_cg(list);
		emit(cal, lexlevel - table[index].level, table[index].addr, "cal", code); // emit cal m = procedure's m from the symbol table
  }

  if(token_cg == beginsym) {
    get_token_cg(list);
    statement_cg(lexlevel, code, list, table);
    while(token_cg == semicolonsym) {
      get_token_cg(list);
      statement_cg(lexlevel, code, list, table);
    }
    get_token_cg(list);
  }

  if(token_cg == ifsym) {
    get_token_cg(list);
    condition_cg(lexlevel, code, list, table);
    int temp_index2 = cx; // save the code index (this is the JPC you need to edit)
    emit(jpc, 0, 0, "jpc", code); // emit JPC, M = 0
    get_token_cg(list);
    statement_cg(lexlevel, code, list, table);

    if(token_cg == elsesym) {
      get_token_cg(list);
      int temp_index3 = cx; // save the current code index (this is the jmp you need to edit)
      emit(jmp, 0, 0, "jmp", code); // emit JMP M = 0
      code[temp_index2].m = cx; // fix the jpc from earlier
      statement_cg(lexlevel, code, list, table);
      code[temp_index3].m = cx; // fix the jmp from earlier
    } else
      code[temp_index2].m = cx; // fix the jpc from earlier
  }

  if(token_cg == whilesym) {
    get_token_cg(list);
    int temp_index3 = cx; // Saves current code index
    condition_cg(lexlevel, code, list, table);
    int temp_index4 = cx; // Saves current code index
    emit(jpc, 0, 0, "jpc", code); // emit JPC, M = 0
    statement_cg(lexlevel, code, list, table);
    emit(jmp, 0, temp_index3, "jmp", code); // emit JMP using the first code index you saved
    code[temp_index4].m = cx; // fix the JPC at the second code index using the current code index
  }

  if(token_cg == readsym) {
    get_token_cg(list);
    int i = symbol_index_cg - 1, index = 0;
    // Save the symbol table index of the entry unmarked and closest in lexlevel.
    while(i >= 0) {
			if(strcmp(identifier, table[i].name) == 0) {
        if (table[i].mark == 0 && table[i].level <= lexlevel) {
					index = i;
					break;
				}
			}
			i--;
		}
    get_token_cg(list);
    emit(sio2, 0, 2, "sio", code); // Read.
    emit(sto, lexlevel - table[index].level, table[index].addr, "sto", code); // Emit store
  }

  if(token_cg == writesym) {
    get_token_cg(list);
    expression_cg(lexlevel, code, list, table);
    emit(sio1, 0, 1, "sio", code); // emit Write.
  }
}

// This function checks to see if the token taken from the lexeme list is either an oddsym, or if it's one of the relation operators.
void condition_cg(int lexlevel, instruction *code, lexeme *list, symbol *table) {
  if(token_cg == oddsym) {
    get_token_cg(list);
    expression_cg(lexlevel, code, list, table);
    emit(opr, 0, odd, "opr", code); // emit odd.
  }
  else {
    expression_cg(lexlevel, code, list, table);
    int relOperator = relation_cg(token_cg);
		get_token_cg(list);
		expression_cg(lexlevel, code, list, table);
		emit(opr, 0, relOperator, "opr", code);
  }
}

// This function checks to see if the token taken from the lexeme list is either a plus or minus sign.
void expression_cg(int lexlevel, instruction *code, lexeme *list, symbol *table) {
  if(token_cg == plussym) {
    get_token_cg(list);
	}
    
  if(token_cg == minussym) {
    get_token_cg(list);
    term_cg(lexlevel, code, list, table);
    emit(opr, 0, neg, "opr", code); // emit negate.
    while(token_cg == plussym || token_cg == minussym) {
      if(token_cg == plussym) {
        get_token_cg(list);
        term_cg(lexlevel, code, list, table);
        emit(opr, 0, add, "opr", code); // emit addition
      }
      if(token_cg == minussym) {
        get_token_cg(list);
        term_cg(lexlevel, code, list, table);
        emit(opr, 0, sub, "opr", code); // emit substraction
      }
    }
    return;
  }
  term_cg(lexlevel, code, list, table);
  while(token_cg == plussym || token_cg == minussym) {
    if(token_cg == plussym) {
      get_token_cg(list);
      term_cg(lexlevel, code, list, table);
      emit(opr, 0, add, "opr", code); // emit addition
    }
    if(token_cg == minussym) {
      get_token_cg(list);
      term_cg(lexlevel, code, list, table);
      emit(opr, 0, sub, "opr", code); // emit subtraction
    }
  }
}

// This function checks to see if the token is either a multiplication or division sign.
void term_cg(int lexlevel, instruction *code, lexeme *list, symbol *table) {
  factor_cg(lexlevel, code, list, table);
  while(token_cg == multsym || token_cg == slashsym) {
    if(token_cg == multsym) {
      get_token_cg(list);
      factor_cg(lexlevel, code, list, table);
      emit(opr, 0, mul, "opr", code); // emit Multi
    }

    if(token_cg == slashsym) {
      get_token_cg(list);
      factor_cg(lexlevel, code, list, table);
      emit(opr, 0, division, "opr", code); // emit Divide
    }
  }
}

// This function checks to see if the token is either an identsym or number.
void factor_cg(int lexlevel, instruction *code, lexeme *list, symbol *table) {
  if(token_cg == identsym) {
    int i = symbol_index_cg - 1, index = 0;
    while(i >= 0) {
			if(strcmp(identifier, table[i].name) == 0) {
        if (table[i].mark == 0 && table[i].level <= lexlevel) {
					index = i;
					break;
				}
			}
			i--;
		} // save the symbol table index of the entry unmarked and closest in lexlevel and not a procedure
    if(table[index].kind == 1) // If the index == constant
      emit(lit, table[index].level, table[index].val, "lit", code);
    if(table[index].kind == 2) // If the index == variable
      emit(lod, lexlevel - table[index].level, table[index].addr, "lod", code);
    get_token_cg(list);
  } else if(token_cg == numbersym) {
		emit(lit, 0, digit_cg, "lit", code);
    get_token_cg(list);
  } else {
    get_token_cg(list);
    expression_cg(lexlevel, code, list, table);
    get_token_cg(list);
  }
}

// Switch statement inside this function returns the different relation_cg operators.
int relation_cg(int token_cg) {
  switch(token_cg) {
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

// This function gets the next token_cg from the lexeme list.
void get_token_cg(lexeme *list) {
	token_cg = list[num_token_cg].type;
	if(token_cg == identsym) {
		int index = 0;
		while(list[num_token_cg].name[index] != '\0') {
			identifier[index] = list[num_token_cg].name[index];
			index++;
		}
		identifier[index] = '\0';
	}

	if(token_cg == numbersym)
		digit_cg = list[num_token_cg].value;

	num_token_cg++;
}

// This function sets the code line to whatever code we have generated.
void emit(int opcode, int l, int m, char *op, instruction* code) {
	// If we are given more code than the max code, throw an error. 
  if (cx > MAX_CODE_LENGTH) { 
    cx++;
    printf("Error: Too much code.");
		exit(-1);
  } else {
    code[cx].opcode = opcode; // Operation name
    code[cx].l = l;						// Lexicographical level
    code[cx].m = m;   				// Modifier
		code[cx].op = op; 				// Opcode
		cx++;
  }
}

void print_assembly(instruction *code){ // Prints the code.
	printf("\n-a compiler directive:\n");
	printf("Generated Assembly Code :\n");
	printf("Line\tOP\t\tL\t\tM\n");
	int code_index = 0;

	while (code_index != cx) {
		printf("%d\t\t%s\t\t%d\t\t%d\n", code_index, code[code_index].op, code[code_index].l, code[code_index].m);
		code_index++;
	}
	printf("\n");
}