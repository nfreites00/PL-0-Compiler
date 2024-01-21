#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser_codegen.h"
#include "lex.h"

#define MAX_CODE_LENGTH 500
#define MAX_SYMBOL_TABLE_SIZE 100

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE]; // Symbols in the symbol table.

// Global variables to keep track of tokens; amounts of tokens and vars; and indexes.
int token, cx, temp_index, var_cnt, symbol_index, numtoken = 0, digit; 

char identifier[11]; // Global array holding name of identifier/

instruction *parse_codegen(lexeme *list, int aFlag) {
	instruction *code = calloc(MAX_CODE_LENGTH, sizeof(instruction));
  program(list, code); // Starts the program.

  // If it gets to this point, the program has no errors.
  printf("\nNo errors, program is syntactically correct.\n");
	if (aFlag == 1) // Flag to print the code.
		printAssembly(code);
  return code;
}

void program(lexeme *list, instruction *code) {
  block(list, code);
	if (token != periodsym) // If no period at the end, then error.
    error(6);
  getToken(list);
  if(token != -1)	// If there is still a token after period, then error.
    error(17);
  emit(sio3, 0, 3, "sio", code); // Emit halt.
}

// This function checks to see if the token that is taken from the lexeme list is either a constant or a variable.
void block(lexeme *list, instruction *code) {
	getToken(list);
  if(token == constsym) { // Const declaration.
    getToken(list);
    if(token != identsym)
      error(4);
    if(searchSymbol(identifier) == 1) // If the identifier = constant then error.
      error(16);
    setName();
    getToken(list);
    if(token != eqlsym) { // Token should = eqlsym, so if not, then error.
      if(token == becomessym)
        error(1);
      else
        error(3);
    }
    getToken(list);
    if(token != numbersym)
      error(2);
		symbolTableAdd(1, 0, 0, digit, 0); // Add the numbersym to the symbol table.
		symbol_index++; // Increase the symbol index.

		// Adds more constants to symbol table.
    getToken(list);
    while(token == commasym) {
      getToken(list);
			if(token != identsym)
				error(4);
			if(searchSymbol(identifier) == 1)
				error(16);
			setName();
			getToken(list);
			if(token != eqlsym) {
				if(token == becomessym)
					error(1);
				else
					error(3);
			}
			getToken(list);
			if(token != numbersym)
				error(2);

      // Add the numbersym to the symbol table.
			symbolTableAdd(1, 0, 0, digit, 0);
			symbol_index++;

      getToken(list);
    }
    if(token != semicolonsym)
      error(5);
    getToken(list);
  }
	
  if(token == varsym) { // Variable declaration.
  	var_cnt = 1; // Keeps track of variables.
    getToken(list);
    if(token != identsym)
      error(4);
    if(searchSymbol(identifier) == 1)
      error(16);

		setName();
		symbolTableAdd(2, 0, var_cnt + 3, 0, 0); // Add the vars to the symbol table.
		symbol_index++;

		// Adds more variables to symbol table.
		getToken(list);
		while(token == commasym) {
			getToken(list);
			if(token != identsym)
				error(4);
			if(searchSymbol(identifier) == 1) // if identifier = constant, then error.
				error(16);
			var_cnt++;

			setName();
			symbolTableAdd(2, 0, var_cnt + 3, 0, 0); // Add the vars to the symbol table.
			symbol_index++;

			getToken(list);
		}
		if(token != semicolonsym) // looking for semicolon
			error(5);
		getToken(list);
  }
  emit(inc, 0, 4 + var_cnt, "inc", code); // Emit (increase using the number of variable).
	statement(list, code);
}

// This function checks to see if the token taken from the lexeme list is either an identsym, a beginsym, an ifsym, or a whilesym.
void statement(lexeme *list, instruction *code) {
  if(token == identsym) {
    int index = 0, i = 0;
    if(searchSymbol(identifier) == 0) // checks if symbol is already in symbol table
      error(7);
    while(i <= symbol_index) {
			if(strcmp(identifier, symbol_table[i].name) == 0) { // looks for index of symbol
        index = i;
				break;
			}
			else if((strcmp(identifier, symbol_table[i].name) != 0) && (i == symbol_index)) // can't find symbol in symbol table
				error(7);
			i++;
		}
    if(symbol_table[index].kind == 1) // if symbol is constant, error.
      error(8);
    getToken(list);
    if(token != becomessym)
      error(9);
    getToken(list);
    expression(list, code);
    emit(sto, 0, symbol_table[index].addr, "sto", code);
  }

  if(token == beginsym) {
    getToken(list);
    statement(list, code);
    while(token == semicolonsym) {
      getToken(list);
      statement(list, code);
		}
    if(token != endsym) // checks for end
      error(11);
		getToken(list);
  }

  if(token == ifsym) { // If token an if, then we emit jpc.
    getToken(list);
    condition(list, code);
    int temp_index2 = cx;
		emit(jpc, 0, 0, "jpc", code);
    if(token != thensym)
      error(10);
    getToken(list);
    statement(list, code);
    code[temp_index2].m = cx; // We fix the m value at the saved code index.
  }

  if(token == whilesym) {
    getToken(list);
    int temp_index3 = cx; // saves current code index
    condition(list, code);
    if(token != dosym) // checks for do
      error(12);
    getToken(list);
    int temp_index4 = cx; // saves current code index
    emit(jpc, 0, 0, "jpc", code);
    statement(list, code);
		emit(jmp, 0, temp_index3, "jmp", code);
    code[temp_index4].m = cx; // sets jpc's M to current code index
  }

  if(token == readsym) {
    getToken(list);
    if(token != identsym)
      error(4);
    getToken(list);
		int i = 0, index;
    while(i <= symbol_index) {
			if(strcmp(identifier, symbol_table[i].name) == 0) {
        index = i;
				break;
			}
			else if((strcmp(identifier, symbol_table[i].name) != 0) && (i == symbol_index))
				error(7);
			i++;
		}
		if(symbol_table[index].kind == 1) // Constant.
      error(8);
    emit(sio2, 0, 2, "sio", code); // Read.
    emit(sto, symbol_table[index].level, symbol_table[index].addr, "sto", code); // Store
  }

  if(token == writesym){
    getToken(list);
    if(token != identsym)
      error(4);
    if(searchSymbol(identifier) == 0) // If symbol is not in search table.
      error(7);
		int i = 0, index;
    while(i <= symbol_index) { // Copy the name of the symbol.
			if(strcmp(identifier, symbol_table[i].name) == 0) {
        index = i;
				break;
			}
		
    // If the identifier and index name are different, then error.
			else if((strcmp(identifier, symbol_table[i].name) != 0) && (i == symbol_index)) 
				error(7);
			i++;
		}
    if(symbol_table[index].kind == 1) // Constant
      emit(lit, 0, symbol_table[index].addr, "lit", code); // LIT
    if(symbol_table[index].kind == 2) // Variable
      emit(lod, 0, symbol_table[index].addr, "lod", code); // LOD
    getToken(list);
    emit(sio1, 0, 1, "sio", code); // Write.
  }
}

// This function checks to see if the token taken from the lexeme list is either an oddsym, or if it's one of the relation operators.  
void condition(lexeme *list, instruction *code) {
  if(token == oddsym) {
    getToken(list);
    expression(list, code);
    emit(opr, 0, odd, "opr", code); // Emit odd.
  }
  else {
    expression(list, code);
    int relOp = relation(token); // Saves the relation operator.
    if (relOp == 0)
      error(13);
    getToken(list);
    expression(list, code);
    emit(opr, 0, relOp, "opr", code); // Emit based on the operator.
  }
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

// This function checks to see if the token taken from the lexeme list is either a plus or minus sign.
void expression(lexeme *list, instruction *code) {
	if(token == plussym) {
		getToken(list);	
    }
	if(token == minussym) {
		getToken(list);
		term(list, code);
		emit(opr, 0, neg, "opr", code); // Emit negate.

		while(token == plussym || token == minussym) {
			if(token == plussym) {
				getToken(list);
				term(list, code);
				emit(opr, 0, add, "opr", code);
			}
			if(token == minussym) {
				getToken(list);
				term(list, code);
				emit(opr, 0, sub, "opr", code);
			}
		}
		return;
	}
	term(list, code);

	while(token == plussym || token == minussym) {
		if(token == plussym) {
			getToken(list);
			term(list, code);
			emit(opr, 0, add, "opr", code);
		}
		if(token == minussym) {
			getToken(list);
			term(list, code);
			emit(opr, 0, sub, "opr", code);
		}
	}
}

// This function checks to see if the token taken from the lexeme list is either a multiplication or division sign.
void term(lexeme *list, instruction *code) {
	factor(list, code);
	while(token == multsym || token == slashsym) {
		if(token == multsym) {
			getToken(list);
			factor(list, code);
			emit(opr, 0, mul, "opr", code); // Emit Multi
		}

		if(token == slashsym) {
			getToken(list);
			factor(list, code);
			emit(opr, 0, division, "opr", code); // Emit Divide
		}
	}
}

// This function checks to see if the token taken from the lexeme list is either an identsym, number, or a pair of parenthesis.
void factor(lexeme *list, instruction *code) {
  if(token == identsym || token == numbersym || token == lparentsym){
		if(token == identsym) {
			int i = 0, index;
			while(i <= symbol_index) {
				if(strcmp(identifier, symbol_table[i].name) == 0) {
					index = i;
					break;
				}
				else if((strcmp(identifier, symbol_table[i].name) != 0) && (i == symbol_index))
					error(7);
				i++;
			}
			if(symbol_table[index].kind == 1) { // If the index = constant
				emit(lit, 0, symbol_table[index].val, "lit", code); // LIT, M = value from symbol table
			}
			if(symbol_table[index].kind == 2) // If the index = variable
				emit(lod, 0, symbol_table[index].addr, "lod", code); // LOD, L & M from symbol table
			getToken(list);
			if (token == numbersym) {
				error(18);
			}
		}	
		
		if(token == numbersym) { 
			emit(lit, 0, digit, "lit", code);
			getToken(list);
		}
		else if (token == identsym || token == numbersym || token == lparentsym) {
			if (token == lparentsym) {
				getToken(list);
				expression(list, code);
				if(token != rparentsym)
					error(14);
				getToken(list);
			}
			else {
				getToken(list);
				expression(list, code);
				getToken(list);
			}
		}
	}
	else
		error(15);
}

// This function sets the code line to whatever code we have generated.
void emit(int opcode, int l, int m, char *op, instruction* code) {
  if (cx > MAX_CODE_LENGTH) { 
    // If we are given more code than the max code, throw an error. 
    printf("Error: Too much code.");
		exit(-1);
  }
  else {
    code[cx].opcode = opcode; // Opcode
    code[cx].l = l;   // Lexicographical level
    code[cx].m = m;   // Modifier
		code[cx].op = op;
    cx++;
  }
}

// This function gets the next token from the lexeme list.
void getToken(lexeme *list) {
	token = list[numtoken].type;
	if(token == identsym) {
		int index = 0;
		while(list[numtoken].name[index] != '\0') {
			identifier[index] = list[numtoken].name[index];
			index++;
		}
		identifier[index] = '\0';
	}

	if(token == numbersym)
		digit = list[numtoken].value;

	numtoken++;
}

// This function searches for the symbol in the symbol table and returns a 1(true) if the symbol is in the symbol table and a 0(false) if not.
int searchSymbol(char name[11]) {
  int i = 0;
  while(i <= symbol_index) { // Return if symbol is in symbol table.
		if(strcmp(name, symbol_table[i].name) == 0)
    	return 1;
		i++;
	}
	return 0;
}

// Adds the kind, and value for constants
// Adds the kind, L (level), and M (address) for variables.
void symbolTableAdd(int kind, int l, int m, int val, int mark) {
		symbol_table[symbol_index].kind = kind;
    symbol_table[symbol_index].val = val;
    symbol_table[symbol_index].level = l;
    symbol_table[symbol_index].addr = m;
    symbol_table[symbol_index].mark = mark;
}

// Sets the name of the symbol in the symbol table.
void setName() {
	int index = 0;
	while(identifier[index] != '\0') {
		symbol_table[symbol_index].name[index] = identifier[index];
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
      printf("Error 16: Symbol already in symbol table.\n");
			break;
    case 17:
      printf("Error 17: Unexpected token after period.\n");
			break;
    case 18:
      printf("Error 18: Expected operation after identifier.\n");
      break;
    default:
      break;
  }
  exit(0); // Stops program when error occurs.
}

void printAssembly(instruction *code){ // Prints the code.
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