// Assignment 4: Homework 4 (PL/0 Compiler)
// Students: Nicole Freites & Shivani Kumar

// DISCLAIMER: parser.c and codegen.c were written using the TA Noelle's pseudocode for the parser and code generator.
//						 Other resources given by the TA Noelle on WebCourses were also used to complete the assignment.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"
#include "codegen.h"
#include "vm.h"

#define MAX_SYMBOL_TABLE_SIZE 500
#define MAX_CODE_LENGTH 500

int main(int argc, char **argv) {
	FILE *ifp = fopen(argv[1], "r");

	if(ifp == NULL || argc == 1) {
		printf("Input file not found.\n");
		exit(-1);
	}

	int aFlag = 0, vFlag = 0, lFlag = 0;

	if (argc == 5) {
		aFlag = 1; vFlag = 1; lFlag = 1;
	}

	else if (argc != 2) {
		if (argv[2][1] == 'v')
				vFlag = 1;
		else if (argv[2][1] == 'a')
				aFlag = 1;
		else 
				lFlag = 1;

		if (argc == 4) {
				if (argv[3][1] == 'v')
						vFlag = 1;
				else if (argv[3][1] == 'a')
						aFlag = 1;
				else 
						lFlag = 1;
		}
	}

  char *charInputArr = calloc(MAX_CODE_LENGTH, sizeof(char));
	int charArr_index = 0;
  char c = fgetc(ifp);
	int i = 0;

	printf("Input File:\n");	
	while(1) {
		charInputArr[charArr_index] = c;
		printf("%c", charInputArr[charArr_index]);
		c = fgetc(ifp);
		if(c == EOF)
			break;
		charArr_index++;
	}
	printf("\n");
	charInputArr[charArr_index + 1] = '\0';

  lexeme *list = lex_analyze(charInputArr, lFlag);
	symbol *table = parser(list);
	instruction *code = codegen(table, list, aFlag);
  virtual_machine(code, vFlag); 

	fclose(ifp);

  return 0;
}