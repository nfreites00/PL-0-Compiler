// Assignment: Homework #2 (Lexical Analyzer)
// Students: Nicole Freites & Shivani Kumar

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_CODE_LENGTH 500

// Holds all the values for the tokens.
typedef enum {
  nulsym = 1, identsym, numbersym, plussym, minussym,
  multsym, slashsym, oddsym, eqlsym, neqsym,
  lessym, leqsym, gtrsym, geqsym, lparentsym,
  rparentsym, commasym, semicolonsym, periodsym, becomessym,
  lbracesym, rbracesym, ifsym, thensym, whilesym,
  dosym, callsym, constsym, varsym, procsym,
  writesym, readsym, elsesym 
} token_type;

// Struct to keep track of tokens when printing lexeme list.
typedef struct lexeme_list {
  int token;
  char identifier[11];
  int digit;
}lexeme_list;


int main(int argc, char ** argv)
{
	// An array that holds tokens for the lexeme list.
	lexeme_list *lexemeArray = calloc(MAX_CODE_LENGTH, sizeof(lexeme_list));

	// String array that holds the reserved words. If a string matches any of the words when being compared, it is a reserved word.
 	const char reserved_words[13][11] = {"const", "var", "procedure", "call", "if", "then", "else", "while", "do", "read", "write", "odd", "null"};

	// Char array to hold all of the characters in the input file.
 	char *charInputArr = calloc(MAX_CODE_LENGTH, sizeof(char));

	// Initializes buffer array to temporarily hold characters when reading through the file. Also, sets buffer to empty.
 	char buffer[11];
	buffer[0] = '\0';

	 printf("Source Program:\n");
	 int charArr_index = 0;

	 FILE *fp = fopen(argv[1], "r");

  	// Reads through input file until it reaches the end of the file.
	while(!feof(fp))
	{
		// Takes in character from input file and places it in char array. Prints it after.
		charInputArr[charArr_index] = fgetc(fp);
		if(charInputArr[charArr_index] == EOF)
			break;

		printf("%c", charInputArr[charArr_index]);
		charArr_index++;
	}

	fclose(fp);

	//Resets and initializes ints to keep track of indexes for the arrays.
  	charArr_index = 0;
	int buff_index = 0;
	int ll_index = 0;

	printf("\n\nLexeme Table:\n");
	printf("lexeme\t\t\ttoken type\n");
  
	// Checks each character in character array until it reaches end-of-file to assign them tokens.
	while(charInputArr[charArr_index] != EOF)
	{
		// Resets index and array, as a precaution.
		buff_index = 0;
		buffer[0] = '\0';

		// Determines if current character is a space or a control character.
		if((isspace(charInputArr[charArr_index]) != 0) || (iscntrl(charInputArr[charArr_index]) != 0))
		{	
			charArr_index++;
			continue;
		}

		// Checks if current char is an letter.
		else if((isalpha(charInputArr[charArr_index]) != 0))
		{
			buff_index = 0;

			// Runs loop until it detects that the current char is not a letter or a digit.
			while((isalpha(charInputArr[charArr_index]) != 0 || isdigit(charInputArr[charArr_index]) != 0) && (isspace(charInputArr[charArr_index]) == 0))
			{
				// Makes sure identifier is not over 11 letters.
				if(buff_index < 11)
				{
					buffer[buff_index] = charInputArr[charArr_index];
					charArr_index++;
					buff_index++;
				}

				// If it's over 11 letters, there is an error that the identifier name is too long.
				// Prints error message and stops the program.
				else
				{
					printf("Name too long.\n");
					return 0;
				}
	  		}

			// Adds null terminator to end of characters to signal the length of the identifier in the buffer array. 
			buffer[buff_index] = '\0';

			// Checks if the identifier in buffer is a reserved word.
			for(int check = 0; check <= 12; check++)
			{
				// If it matches one of the reserved words, it will print the reserved word and its token.
				if((strcmp(buffer, reserved_words[check]) == 0) && buffer[0] != '\0')
				{
					switch(check)
					{
						// const
						case 0:
							lexemeArray[ll_index].token = constsym;
							ll_index++;
							printf("const\t\t\t%d\n", constsym);
							buffer[0] = '\0';
							break;

						// var
						case 1:
							lexemeArray[ll_index].token = varsym;
							ll_index++;
							printf("var\t\t\t\t%d\n", varsym);
							buffer[0] = '\0';
							break;

						// procedure
						case 2:
							lexemeArray[ll_index].token = procsym;
							ll_index++;
							printf("procedure\t\t%d\n", procsym);
							buffer[0] = '\0';
							break;

						// call
						case 3:
							lexemeArray[ll_index].token = callsym;
							ll_index++;
							printf("call\t\t\t%d\n", callsym);
							buffer[0] = '\0';
							break;

						// if
						case 4:
							lexemeArray[ll_index].token = ifsym;
							ll_index++;
							printf("if\t\t\t\t%d\n", ifsym);
							buffer[0] = '\0';
							break;

						// then
						case 5:
							lexemeArray[ll_index].token = thensym;
							ll_index++;
							printf("then\t\t\t%d\n", thensym);
							buffer[0] = '\0';
							break;

						// else
						case 6:
							lexemeArray[ll_index].token = elsesym;
							ll_index++;
							printf("else\t\t\t%d\n", elsesym);
							buffer[0] = '\0';
							break;
						
						// while
						case 7:
							lexemeArray[ll_index].token = whilesym;
							ll_index++;
							printf("while\t\t\t%d\n", whilesym);
							buffer[0] = '\0';
							break;

						// do
						case 8:
							lexemeArray[ll_index].token = dosym;
							ll_index++;
							printf("do\t\t\t\t%d\n", dosym);
							buffer[0] = '\0';
							break;

						// read
						case 9:
							lexemeArray[ll_index].token = readsym;
							ll_index++;
							printf("read\t\t\t\t%d\n", readsym);
							buffer[0] = '\0';
							break;

						// write
						case 10:
							lexemeArray[ll_index].token = writesym;
							ll_index++;
							printf("write\t\t\t\t%d\n", writesym);
							buffer[0] = '\0';
							break;						

						// odd
						case 11:
							lexemeArray[ll_index].token = oddsym;
							ll_index++;
							printf("odd\t\t\t\t%d\n", oddsym);
							buffer[0] = '\0';
							break;

						// null
						case 12:
							lexemeArray[ll_index].token = nulsym;
							ll_index++;
							printf("null\t\t\t\t%d\n", nulsym);
							buffer[0] = '\0';
							break;
					}
				}
	    
				// If the identifier in buffer is not a reserved word, it is an identifier.
				else if(check == 12 && (strcmp(buffer, reserved_words[check]) != 0) && buffer[0] != '\0')
				{
					lexemeArray[ll_index].token = identsym;
					int index = 0;

					// Places identifier in lexeme list to print later.
					while(buffer[index] != '\0')
					{
						lexemeArray[ll_index].identifier[index] = buffer[index];
						index++;
					}
					ll_index++;
					printf("%s\t\t\t\t%d\n", buffer, identsym);
					buffer[0] = '\0'; 
				} 
			}
			continue;
		}

		// Determines if the character is a digit.
		else if((isdigit(charInputArr[charArr_index]) != 0) && (isspace(charInputArr[charArr_index]) == 0))
		{	
			// Characters get checked until a non-digit or space is reached.
			while((isdigit(charInputArr[charArr_index]) != 0) && (isspace(charInputArr[charArr_index]) == 0))
			{
				// If an identifier starts with a digit, an error will occur.
				if(isalpha(charInputArr[charArr_index]) != 0)
				{
					printf("Variable does not start with letter.\n");
					return 0;
				}

				// Makes sure the digit is at most 5 digits in length.
				else if(buff_index < 5)
				{
					buffer[buff_index] = charInputArr[charArr_index];
					charArr_index++;
					buff_index++;
				}

				// If the number passes 5 digits, an error will occur.
				else
				{
					printf("Number too long.\n");
					return 0;
				}
			}

			buffer[buff_index] = '\0';

			// Makes sure string is not empty before setting token.
			if(buffer[0] != '\0')
			{
				// Sets token to numbersym.
				lexemeArray[ll_index].token = numbersym;

				// Goes through each digit in buffer and places the digit in the lexeme list placeholder.
				int index = 0;
				lexemeArray[ll_index].digit = atoi(buffer);
				ll_index++;
				printf("%s\t\t\t\t%d\n", buffer, numbersym);
				buffer[0] = '\0';
			}
		}

		// Determines if character is a symbol: if it's not a letter, digit, or space, it's a symbol.
		else if((isalpha(charInputArr[charArr_index]) == 0) && (isdigit(charInputArr[charArr_index]) == 0) && (isspace(charInputArr[charArr_index]) == 0))
		{
			// Goes through all the possibilities of symbols that may be encountered in an input.
			switch(charInputArr[charArr_index])
			{
				// plus
				case '+':
					printf("+\t\t\t\t%d\n", plussym);
					lexemeArray[ll_index].token = plussym;
					ll_index++;
					charArr_index++;
					break;

				// minus
				case '-':
					printf("-\t\t\t\t%d\n", minussym);
					lexemeArray[ll_index].token = minussym;
					ll_index++;
					charArr_index++;
					break;

				// multiplication
				case '*':
					printf("*\t\t\t\t%d\n", multsym);
					lexemeArray[ll_index].token = multsym;
					ll_index++;
					charArr_index++;
					break;
				
				// slash
				case '/':
					charArr_index++;

					// Checks if the next symbol is a *. 
					// If so, it is a comment and will loop through the char array until it finds the end of the comment */.
					if(charInputArr[charArr_index] == '*')
					{
						while(charInputArr[charArr_index] != EOF)
						{
							charArr_index++;
							if(charInputArr[charArr_index] == '*')
							{	
								charArr_index++;
								if(charInputArr[charArr_index] == '/')
								{
									charArr_index++;
									break;
								}
								else
									continue;
							}
						}
					}

					// If it is not a comment, it is just a slash symbol.
					else
					{
						printf("/\t\t\t\t%d\n", slashsym);
						lexemeArray[ll_index].token = slashsym;
						ll_index++;
					}
					break;

				// left parenthesis
				case '(':
					printf("(\t\t\t\t%d\n", lparentsym);
					lexemeArray[ll_index].token = lparentsym;
					ll_index++;
					charArr_index++;
					break;

				// right parenthesis
				case ')':
					printf(")\t\t\t\t%d\n", rparentsym);
					lexemeArray[ll_index].token = rparentsym;
					ll_index++;
					charArr_index++;
					break;

				// equal
				case '=':
					printf("=\t\t\t\t%d\n", eqlsym);
					lexemeArray[ll_index].token = eqlsym;
					ll_index++;
					charArr_index++;
					break;

				// comma
				case ',':
					printf(",\t\t\t\t%d\n", commasym);
					lexemeArray[ll_index].token = commasym;
					ll_index++;
					charArr_index++;
					break;

				// period
				case '.':
					printf(".\t\t\t\t%d\n", periodsym);
					lexemeArray[ll_index].token = periodsym;
					ll_index++;
					charArr_index++;
					break;

				// less than
				case '<':
					charArr_index++;

	     			// Checks to see if the next character makes it a leqsym
					if(charInputArr[charArr_index] == '=')
					{
						printf("<=\t\t\t\t%d\n", leqsym);
						lexemeArray[ll_index].token = leqsym;
						ll_index++;
						charArr_index++;
					}

	      			// Checks to see if the next character makes it a neqsym
					else if(charInputArr[charArr_index] == '>')
					{
						printf("<>\t\t\t\t%d\n", neqsym);
						lexemeArray[ll_index].token = neqsym;
						ll_index++;
						charArr_index++;
					}

	     		 	// If not, it is just the less than symbol.
					else
					{
						printf("<\t\t\t\t%d\n", lessym);
						lexemeArray[ll_index].token = lessym;
						ll_index++;
					}

					break;

				// greater than
				case '>':
					charArr_index++;

	      			// Checks to see if the next character will make it a geqsym.
					if(charInputArr[charArr_index] == '=')
					{
						printf(">=\t\t\t\t%d\n", geqsym);
						lexemeArray[ll_index].token = geqsym;
						ll_index++;
						charArr_index++;
					}

	      			// If not, it is just a greater than symbol.
					else
					{
						printf(">\t\t\t\t%d\n", gtrsym);
						lexemeArray[ll_index].token = gtrsym;
						ll_index++;
					}
					break;

				// semicolon
				case ';':
					printf(";\t\t\t\t%d\n", semicolonsym);
					lexemeArray[ll_index].token = semicolonsym;
					ll_index++;
					charArr_index++;
					break;

				// colon
				case ':':
					charArr_index++;

	      			// Checks the next character to see if it makes it a becomessym.
					if(charInputArr[charArr_index] == '=')
					{
						printf(":=\t\t\t\t%d\n", becomessym);
						lexemeArray[ll_index].token = becomessym;
						ll_index++;
						charArr_index++;
					}
	      
					// If not, the symbol is invalid and throws an error.
					else
					{
						printf("Invalid symbols.\n");
						return 0;
					}
					break;

				// left brace
				case '{':
					printf("{\t\t\t\t%d\n", lbracesym);
					lexemeArray[ll_index].token = lbracesym;
					ll_index++;
					charArr_index++;
					break;

				// right brace
				case '}':
					printf("}\t\t\t\t%d\n", rbracesym);
					lexemeArray[ll_index].token = rbracesym;
					ll_index++;
					charArr_index++;
					break;

				// not
				case '!':
					charArr_index++;

	     	 		// Checks to see if the next character makes it a neqsym.
					if(charInputArr[charArr_index] == '=')
					{
						printf("!=\t\t\t\t%d\n", neqsym);
						lexemeArray[ll_index].token = neqsym;
						ll_index++;
						charArr_index++;
					}

	      			// If not, the symbol is invalid and throws an error.
					else
					{
						printf("Invalid symbols.\n");
						return 0;
					}
					break;

				// If it is not any of the special symbols provided, it is an invalid symbol.
				default:
					printf("Invalid symbols.\n");
					break;
			}
		}
		continue;
	}

	printf("Lexeme List:\n");
	int count = 0;
	lexemeArray[ll_index].token = '\0';

	// Until count does not reach the terminator, it will print all of the corresponding tokens in the lexeme list array.
	while(lexemeArray[count].token != '\0')
	{
		// If the token is an identifier, it will print the corresponding identifier in the list.
		if(lexemeArray[count].token == identsym)
		  printf("%d %s ", lexemeArray[count].token, lexemeArray[count].identifier);

		// If the token is a digit, it will print the corresponding digit in the list.
		else if(lexemeArray[count].token == numbersym)
			printf("%d %d ", lexemeArray[count].token, lexemeArray[count].digit);

		// If the token is not a identifier or digit, it just prints the token alone.
		else
		  printf("%d ", lexemeArray[count].token);

		count++;
	}
	
	printf("\n");

	// Frees the arrays.
	free(charInputArr);
	free(lexemeArray);
	charInputArr = NULL;
	lexemeArray = NULL;

	return 0;
}