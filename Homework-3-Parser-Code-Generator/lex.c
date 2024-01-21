#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lex.h"

#define MAX_CODE_LENGTH 500

int charArr_index = 0, buff_index = 0, ll_index = 0; //Initializes ints to keep track of indexes for the arrays.

lexeme *lex_analyze(char *inputFile, int lFlag) {
  // An array that holds types for the lexeme list.
	lexeme *lexemeArray = calloc(MAX_CODE_LENGTH, sizeof(lexeme));

	if (lFlag == 1) {
		printf("\n-l compiler directive:\n");
    printf("Lexeme Table:\n");
    printf("lexeme\t\ttoken type\n");
	}
    
	// String array that holds the reserved words. If a string matches any of the words when being compared, it is a reserved word.
 	const char reserved_words[13][11] = {"const", "var", "begin", "end", "if", "then", "while", "do", "read", "write", "odd", "null"};

	// Initializes buffer array to temporarily hold characters when reading through the file. Also, sets buffer to empty.
 	char buffer[11];
	buffer[0] = '\0';

	// Checks each character in character array until it reaches end-of-file to assign them tokens.
	while(inputFile[charArr_index] != '\0') {
		// Resets index and array, as a precaution.
		buff_index = 0;
		buffer[0] = '\0';
		// Determines if current character is a space.
		if(isspace(inputFile[charArr_index]) != 0) {	
			charArr_index++;
			continue;
		}

		// Checks if current char is an letter.
		else if((isalpha(inputFile[charArr_index]) != 0) && (isspace(inputFile[charArr_index]) == 0)) {
			buff_index = 0;
			// Runs loop until it detects that the current char is not a letter or a digit.
			while((isalpha(inputFile[charArr_index]) != 0 || isdigit(inputFile[charArr_index]) != 0) && isspace(inputFile[charArr_index]) == 0) {
				if(buff_index < 11) { // Makes sure identifier is not over 11 letters.
					buffer[buff_index] = inputFile[charArr_index];
					charArr_index++;
					buff_index++;
				}

				// If it's over 11 letters, there is an error that the identifier name is too long.
				// Prints error message and stops the program.
				else {
					printf("Error: Name too long.\n");
					exit(-1);
				}
	  	}
			// Adds null terminator to end of characters to signal the length of the identifier in the buffer array. 
			buffer[buff_index] = '\0';

			// Checks if the identifier in buffer is a reserved word.
			for(int check = 0; check <= 12; check++) {
				// If it matches one of the reserved words, it will print the reserved word and its token.
				if((strcmp(buffer, reserved_words[check]) == 0) && buffer[0] != '\0') {
					switch(check) {
						// const
						case 0:
							lexemeArray[ll_index].type = constsym;
							ll_index++;
              if(lFlag == 1) {
                printf("const\t\t\t%d\n", constsym);
              }
							buffer[0] = '\0';
							break;
						// var
						case 1:
							lexemeArray[ll_index].type = varsym;
							ll_index++;
              if(lFlag == 1) {
                printf("var\t\t\t\t%d\n", varsym);
              }
							buffer[0] = '\0';
							break;
						// begin
						case 2:
							lexemeArray[ll_index].type = beginsym;
							ll_index++;
              if(lFlag == 1) {
                printf("begin\t\t\t%d\n", beginsym);
              }
							buffer[0] = '\0';
							break;
						// end
						case 3:
							lexemeArray[ll_index].type = endsym;
							ll_index++;
              if(lFlag == 1) {
                printf("end\t\t\t\t%d\n", endsym);
              }
							buffer[0] = '\0';
							break;
						// if
						case 4:
							lexemeArray[ll_index].type = ifsym;
							ll_index++;
              if(lFlag == 1) {
                printf("if\t\t\t\t%d\n", ifsym);
              }
							buffer[0] = '\0';
							break;
						// then
						case 5:
							lexemeArray[ll_index].type = thensym;
							ll_index++;
              if(lFlag == 1) {
                printf("then\t\t\t%d\n", thensym);
              }
							buffer[0] = '\0';
							break;
						// while
						case 6:
							lexemeArray[ll_index].type = whilesym;
							ll_index++;
              if(lFlag == 1) {
                printf("while\t\t\t%d\n", whilesym);
              }
							buffer[0] = '\0';
							break;

						// do
						case 7:
							lexemeArray[ll_index].type = dosym;
							ll_index++;
              if(lFlag == 1) {
                printf("do\t\t\t\t%d\n", dosym);
              }
							buffer[0] = '\0';
							break;
						// read
						case 8:
							lexemeArray[ll_index].type = readsym;
							ll_index++;
              if(lFlag == 1) {
                printf("read\t\t\t%d\n", readsym);
              }
							buffer[0] = '\0';
							break;
						// write
						case 9:
							lexemeArray[ll_index].type = writesym;
							ll_index++;
              if(lFlag == 1) {
                printf("write\t\t\t%d\n", writesym);
              }
							buffer[0] = '\0';
							break;						
						// odd
						case 10:
							lexemeArray[ll_index].type = oddsym;
							ll_index++;
              if(lFlag == 1) {
                printf("odd\t\t\t\t%d\n", oddsym);
              }
							buffer[0] = '\0';
							break;
						// null
						case 11:
							lexemeArray[ll_index].type = nulsym;
							ll_index++;
              if(lFlag == 1) {
                printf("NULL\t\t\t\t%d\n", nulsym);
              }
							buffer[0] = '\0';
							break;
					}
        }
	    
				// If the identifier in buffer is not a reserved word, it is an identifier.
				else if(check == 12 && (strcmp(buffer, reserved_words[check]) != 0) && buffer[0] != '\0') {
					lexemeArray[ll_index].type = identsym;
					int index = 0;
					// Places identifier in lexeme list to print later.
					while(buffer[index] != '\0') {
						lexemeArray[ll_index].name[index] = buffer[index];
						index++;
					}
					lexemeArray[ll_index].name[index] = '\0';
          if(lFlag == 1) {
            printf("%s\t\t\t\t%d\n", lexemeArray[ll_index].name, identsym);
          }
					ll_index++;
					buffer[0] = '\0'; 
				} 
			}
			continue;
		}

		else if((isdigit(inputFile[charArr_index]) != 0) && (isspace(inputFile[charArr_index]) == 0)) { // determines if character is a digit
			// Characters get checked until a non-digit or space is reached.
			while((isdigit(inputFile[charArr_index]) != 0 || isalpha(inputFile[charArr_index]) != 0) && (isspace(inputFile[charArr_index]) == 0)) {
				// If an identifier starts with a digit, an error will occur.
				if(isalpha(inputFile[charArr_index]) != 0) {
					printf("Error: Variable does not start with letter.\n");
					exit(-1);
				}

				else if(buff_index < 5) { // Makes sure the digit is at most 5 digits in length.
					buffer[buff_index] = inputFile[charArr_index];
					charArr_index++;
					buff_index++;
				}

				else { // If the number passes 5 digits, an error will occur.
					printf("Error: Number too long.\n");
					exit(-1);
				}
			}

			buffer[buff_index] = '\0';

			if(buffer[0] != '\0') { // Makes sure string is not empty before setting token.
				lexemeArray[ll_index].type = numbersym; // Sets token to numbersym.

				// Goes through each digit in buffer and places the digit in the lexeme list placeholder.
				int index = 0;
				lexemeArray[ll_index].value = atoi(buffer);
				ll_index++;
        if(lFlag == 1) {
          printf("%s\t\t\t\t%d\n", buffer, numbersym);
        }
				buffer[0] = '\0';
			}
		}

		// Determines if character is a symbol: if it's not a letter, digit, or space, it's a symbol.
		else if((isalpha(inputFile[charArr_index]) == 0) && (isdigit(inputFile[charArr_index]) == 0) && (isspace(inputFile[charArr_index]) == 0)) {
			// Goes through all the possibilities of symbols that may be encountered in an input.
			switch(inputFile[charArr_index]) {
				case '+': // plus
          if(lFlag == 1) {
            printf("+\t\t\t\t%d\n", plussym);
          }
					lexemeArray[ll_index].type = plussym;
					ll_index++;
					charArr_index++;
					break;
				// minus
				case '-':
          if(lFlag == 1) {
            printf("-\t\t\t\t%d\n", minussym);
          }
					lexemeArray[ll_index].type = minussym;
					ll_index++;
					charArr_index++;
					break;
				// multiplication
				case '*':
          if(lFlag == 1) {
            printf("*\t\t\t\t%d\n", multsym);
          }
					lexemeArray[ll_index].type = multsym;
					ll_index++;
					charArr_index++;
					break;
				// slash
				case '/':
					charArr_index++;

					// Checks if the next symbol is a *. 
					// If so, it is a comment and will loop through the char array until it finds the end of the comment */.
					if(inputFile[charArr_index] == '*') {
						while(inputFile[charArr_index] != EOF) {
							charArr_index++;
							if(inputFile[charArr_index] == '*') {	
								charArr_index++;
								if(inputFile[charArr_index] == '/') {
									charArr_index++;
									break;
								}
								else
									continue;
							}
						}
					}

					// If it is not a comment, it is just a slash symbol.
					else {
            if(lFlag == 1) {
              printf("/\t\t\t\t%d\n", slashsym);
            }
						lexemeArray[ll_index].type = slashsym;
						ll_index++;
					}
					break;

				// left parenthesis
				case '(':
          if(lFlag == 1) {
            printf("(\t\t\t\t%d\n", lparentsym);
          }
					lexemeArray[ll_index].type = lparentsym;
					ll_index++;
					charArr_index++;
					break;

				// right parenthesis
				case ')':
          if(lFlag == 1) {
            printf(")\t\t\t\t%d\n", rparentsym);
          }
					lexemeArray[ll_index].type = rparentsym;
					ll_index++;
					charArr_index++;
					break;

				// equal
				case '=':
          if(lFlag == 1) {
            printf("=\t\t\t\t%d\n", eqlsym);
          }
					lexemeArray[ll_index].type = eqlsym;
					ll_index++;
					charArr_index++;
					break;

				// comma
				case ',':
          if(lFlag == 1) {
            printf(",\t\t\t\t%d\n", commasym);
          }
					lexemeArray[ll_index].type = commasym;
					ll_index++;
					charArr_index++;
					break;

				// period
				case '.':
          if(lFlag == 1) {
            printf(".\t\t\t\t%d\n", periodsym);
          }
					lexemeArray[ll_index].type = periodsym;
					ll_index++;
					charArr_index++;
					break;

				// less than
				case '<':
					charArr_index++;

	     			// Checks to see if the next character makes it a leqsym
					if(inputFile[charArr_index] == '=') {
            if(lFlag == 1) {
              printf("<=\t\t\t\t%d\n", leqsym);
            }
						lexemeArray[ll_index].type = leqsym;
						ll_index++;
						charArr_index++;
					}

	      			// Checks to see if the next character makes it a neqsym
					else if(inputFile[charArr_index] == '>') {
            if(lFlag == 1) {
              printf("<>\t\t\t\t%d\n", neqsym);
            }
						lexemeArray[ll_index].type = neqsym;
						ll_index++;
						charArr_index++;
					}

	     		 	// If not, it is just the less than symbol.
					else {
            if(lFlag == 1) {
              printf("<\t\t\t\t%d\n", lessym);
            }
						lexemeArray[ll_index].type = lessym;
						ll_index++;
					}

					break;

				// greater than
				case '>':
					charArr_index++;

	      			// Checks to see if the next character will make it a geqsym.
					if(inputFile[charArr_index] == '=') {
            if(lFlag == 1) {
              printf(">=\t\t\t\t%d\n", geqsym);
            }
						lexemeArray[ll_index].type = geqsym;
						ll_index++;
						charArr_index++;
					}
					else // If not, it is just a greater than symbol.
          {
            if(lFlag == 1) {
              printf(">\t\t\t\t%d\n", gtrsym);
            }
						lexemeArray[ll_index].type = gtrsym;
						ll_index++;
          }
					break;

				// semicolon
				case ';':
          if(lFlag == 1) {
            printf(";\t\t\t\t%d\n", semicolonsym);
          }
					lexemeArray[ll_index].type = semicolonsym;
					ll_index++;
					charArr_index++;
					break;

				// colon
				case ':':
					charArr_index++;

	      			// Checks the next character to see if it makes it a becomessym.
					if(inputFile[charArr_index] == '=') {
            if(lFlag == 1) {
              printf(":=\t\t\t\t%d\n", becomessym);
            }
						lexemeArray[ll_index].type = becomessym;
						ll_index++;
						charArr_index++;
					}
					else {// If not, the symbol is invalid and throws an error. 
						printf("Error: Invalid symbols.\n");
						exit(-1);
          }
					break;

				// not
				case '!':
					charArr_index++;

	     	 		// Checks to see if the next character makes it a neqsym.
					if(inputFile[charArr_index] == '=') {
            if(lFlag == 1) {
              printf("!=\t\t\t\t%d\n", neqsym);
            }
						lexemeArray[ll_index].type = neqsym;
						ll_index++;
						charArr_index++;
					}

					else { // If not, the symbol is invalid and throws an error.
						printf("Error: Invalid symbols.\n");
						exit(-1);
          }
					break;

				// If it is not any of the special symbols provided, it is an invalid symbol.
				default:
					printf("Error: Invalid symbols.\n");
					exit(-1);
					break;
			}
		}
		continue;
	}
	lexemeArray[ll_index].type = -1;
	if (lFlag == 1) {
		printList(lexemeArray);
	}
  return lexemeArray;
}

void printTable(lexeme *lexemeArray, int count) {
  printf("%s %d ", lexemeArray[count].name, lexemeArray[count].type);
}

void printList(lexeme *lexemeArray) {
	printf("\nLexeme List:\n");
	int count = 0;
  lexemeArray[ll_index].type = -1;

  while(lexemeArray[count].type != -1) {
    if(lexemeArray[count].type == identsym)
      printf("%d %s ", lexemeArray[count].type, lexemeArray[count].name);

    else if(lexemeArray[count].type == numbersym)
      printf("%d %d ", lexemeArray[count].type, lexemeArray[count].value);
    
    else
      printf("%d ", lexemeArray[count].type);
    
    count++;
  }
    printf("\n");
}