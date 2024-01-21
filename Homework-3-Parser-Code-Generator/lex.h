#ifndef __LEX_H
#define __LEX_H

// Holds all the values for the tokens.
typedef enum {
  nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym, oddsym, eqlsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym, whilesym, dosym, temp1, constsym, varsym, temp2, writesym, readsym, temp3
} token_type;

// Struct that holds values for each item in the lexeme list.
typedef struct lexeme {
  char name[11];
  int value;
  int type;
} lexeme;

lexeme *lex_analyze(char *inputfile, int lFlag);
void printTable(lexeme *lexemeArray, int count);
void printList(lexeme *lexemeArray);
#endif