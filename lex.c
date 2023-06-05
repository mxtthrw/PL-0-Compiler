/*
  Authors: Nathan Manierre and Matthew Dahbar

	This is the skeleton lex.c for the UCF Fall 2021 Systems Software 
	Project, HW4. Add whatever you want, but make sure to leave the
	print functions as they are to make sure you get your grade as
	quickly as possible. There is a line commented out just before
	the return statement in lexanalyzer(). Make sure to uncomment it
	after you implement the function, your parser.c may malfunction
	without it.
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "compiler.h"
#include <math.h>

#define MAX_NUMBER_TOKENS 1000
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5
#define MAX_CODE_LENGTH 1000

lexeme *list;
int lex_index;
void printlexerror(int type);
void printtokens();

// all special words
char *var = "var";
char *when = "when";    //new if
char *constt = "const";
char *procedure = "procedure";
char *call = "call";
char *elsedo = "elsedo";  //new else
char *whilee = "while";
char *doo = "do";
char *od = "od";  //new end
char *read = "read";
char *write = "write";
char *odd = "odd";


bool issymbol(char ch)
{
	bool ret = false;
  if(ch == '!') ret = true;
  if(ch == '<') ret = true;
  if(ch == '>') ret = true;
  if(ch == '(') ret = true;
  if(ch == ')') ret = true;
  if(ch == '.') ret = true;
  if(ch == ',') ret = true;
  if(ch == '*') ret = true;
  if(ch == ':') ret = true;
	if(ch == '+') ret = true;
	if(ch == '-') ret = true;
	if(ch == '=') ret = true;
	if(ch == '%') ret = true;
	if(ch == '/') ret = true; 
	if(ch == ';') ret = true;
	return ret;
}


lexeme *lexanalyzer(char *input, int printFlag)
{
	list = NULL;	
	list = malloc(MAX_NUMBER_TOKENS * sizeof(char));
	
	for(int i = 0; i < MAX_NUMBER_TOKENS; i++)
	{
		// cntrl char
		if (iscntrl(input[i]))
			i++;

		// space
		if (isspace(input[i]))
		{
			i++;
		}	

    
		if(input[i] == '/' && input[i+1] == '/')
		{
			while(!iscntrl(input[i]))
			{
				i += 1;
			}
			i++;
		}

		// starts with a letter
		if (isalpha(input[i]))
		{
			char *beginsletter = malloc(11 * sizeof(char));
			int j = 0;
			
			while(isalnum(input[i+j]) && ((i+j) < MAX_CODE_LENGTH)) 
			{
				if(isalnum(input[i+j]))
				{
					beginsletter[j] = input[i+j];
				}
				j++;

				if(j > 10)
				{
					// error too many characters
					printlexerror(4);		
					// free all of the chars
					free(beginsletter);
					return NULL;
				}
			}
			if(!strcmp(beginsletter, var))
			{
				// list[lex_index].value = 2;
				list[lex_index].type = varsym;
				strcpy(list[lex_index].name, var);
				lex_index++;
			}
			else if(!strcmp(beginsletter, constt))
			{
				// list[lex_index].value = 1;
				list[lex_index].type = constsym;
				strcpy(list[lex_index].name, constt);
				lex_index++;
			}
			else if(!strcmp(beginsletter, procedure))
			{
				// list[lex_index].value = 3;
				list[lex_index].type = procsym;
        strcpy(list[lex_index].name, procedure);
				lex_index++;	
			}
			else if(!strcmp(beginsletter, call))
			{
				// list[lex_index].value = 11;
				list[lex_index].type = callsym;
        strcpy(list[lex_index].name, call);
				lex_index++;
			}
			else if(!strcmp(beginsletter, when))
			{
				// list[lex_index].value = 8;
				list[lex_index].type = whensym;
        strcpy(list[lex_index].name, when);
				lex_index++;
			}
			else if(!strcmp(beginsletter, doo))
			{
				// list[lex_index].value = 9;
				list[lex_index].type = dosym;
        strcpy(list[lex_index].name, doo);
				lex_index++;
			}
			else if(!strcmp(beginsletter, elsedo))
			{
				// list[lex_index].value = 10;
				list[lex_index].type = elsedosym;
        strcpy(list[lex_index].name, elsedo);
				lex_index++;
			}
			else if(!strcmp(beginsletter, whilee))
			{
				// list[lex_index].value = 6;
				list[lex_index].type = whilesym;
        strcpy(list[lex_index].name, whilee);
				lex_index++;
			}
			else if(!strcmp(beginsletter, doo))
			{
				// list[lex_index].value = 7;
				list[lex_index].type = dosym;
        strcpy(list[lex_index].name, doo);
				lex_index++;
			}
			else if(!strcmp(beginsletter, doo))
			{
				// list[lex_index].value = 4;
				list[lex_index].type = dosym;
        strcpy(list[lex_index].name, doo);
				lex_index++;
			}
			else if(!strcmp(beginsletter, od))
			{
				// list[lex_index].value = 5;
				list[lex_index].type = odsym;
        strcpy(list[lex_index].name, od);
				lex_index++;
			}
			else if(!strcmp(beginsletter, read))
			{
				// list[lex_index].value = 13;
				list[lex_index].type = readsym;
        strcpy(list[lex_index].name, read);
				lex_index++;
			}
			else if(!strcmp(beginsletter, write))
			{
				// list[lex_index].value = 12;
				list[lex_index].type = writesym;
        strcpy(list[lex_index].name, write);
				lex_index++;
			}
			else if(!strcmp(beginsletter, odd))
			{
				// list[lex_index].value = 28;
				list[lex_index].type = oddsym;
        strcpy(list[lex_index].name, odd);
				lex_index++;
			}
			else
			{
				if(strcmp(beginsletter, ""))
				{
					list[lex_index].type = identsym;
					strcpy(list[lex_index].name, beginsletter);
					lex_index++;
				}

        // else
        // {
          
        // }

				// printf("*%s*\n", beginsletter);
				// list[lex_index].value = 14;
				
			}
			// i = i+j - 1;
			i = i+j;
		}

		// starts with a number
		if (isdigit(input[i]))
		{
			int *beginsnum = malloc(5 * sizeof(int));
			int j = 0;
			
			while(isalnum(input[i+j]) && ((i+j) < MAX_CODE_LENGTH))
			{
				// error: if letter
				if(isalpha(input[i+j]))
				{
					printlexerror(2);
					free(beginsnum);
					return NULL;
				}
				if(isdigit(input[i+j]))
				{
					beginsnum[j] = input[i+j]-'0';
				}
				j++;

				if(j > 4)
				{
					// error too many characters
					printlexerror(3);		
					// free all of the chars
					free(beginsnum);
					return NULL;
				}
			}
			int val = 0;
			for(int k = 0; k < j; k++)
			{
				val += (beginsnum[k] * pow(10,j-k-1));
			}
			list[lex_index].value = val;
			list[lex_index].type = numbersym;
			lex_index++;
			i = i+j;
		}

		// check for symbol
    if(issymbol(input[i]))
    {
      if(input[i] == '%')
      {
        strcpy(list[lex_index].name, "%");
        list[lex_index].type = modsym;
        lex_index++;
      }
      
			else if(input[i] == '+')
      {
        strcpy(list[lex_index].name, "+");
        list[lex_index].type = addsym;
        lex_index++;
      }
      
			else if(input[i] == ';')
      {
        strcpy(list[lex_index].name, ";");
        list[lex_index].type = semicolonsym;
        lex_index++;
      }
      
			else if(input[i] == '.')
      {
        strcpy(list[lex_index].name, ".");
        list[lex_index].type = periodsym;
        lex_index++;
      }
      else if(input[i] == '*')
      {
        strcpy(list[lex_index].name, "*");
        list[lex_index].type = multsym;
        lex_index++;
      }
      else if(input[i] == '-')
      {
        strcpy(list[lex_index].name, "-");
        list[lex_index].type = subsym;
        lex_index++;
      }
      else if(input[i] == '/')
      {
        strcpy(list[lex_index].name, "/");
        list[lex_index].type = divsym;
        lex_index++;
      }
      else if(input[i] == ',')
      {
        strcpy(list[lex_index].name, ",");
        list[lex_index].type = commasym;
        lex_index++;
      }
      else if(input[i] == '=')
      {
				if (input[i+1] != '=')
        {
					strcpy(list[lex_index].name, "=");
					list[lex_index].type = eqlsym;
					lex_index++;
					i++;
        }
        // if(input[i+1] == '=')
        // {
        //   strcpy(list[lex_index].name, "==");
        //   list[lex_index].type = eqlsym;
        //   lex_index++;
        //   i++;
        // }
      }
			else if(input[i] == '!')
      {
        if(input[i+1] == '=')
        {
          strcpy(list[lex_index].name, "!=");
          list[lex_index].type = neqsym;
          lex_index++;
          i++;
        }
      }
      else if(input[i] == '<')
      {
        if(input[i+1] == '=')
        {
          strcpy(list[lex_index].name, "<=");
          list[lex_index].type = leqsym;
          lex_index++;
          i++;
        }
        else
        {
          strcpy(list[lex_index].name, "<");
          list[lex_index].type = lsssym;
          lex_index++;
        }
      }
      else if(input[i] == '>')
      {
        if(input[i+1] == '=')
        {
          strcpy(list[lex_index].name, ">=");
          list[lex_index].type = geqsym;
          lex_index++;
          i++;
        }
        else
        {
          strcpy(list[lex_index].name, ">");
          list[lex_index].type = gtrsym;
          lex_index++;
        }
      }
      else if(input[i] == ':')
      {
        if(input[i+1] == '=')
        {
          strcpy(list[lex_index].name, ":=");
          list[lex_index].type = assignsym;
          lex_index++;
          i++;
        }
      }
      else if(input[i] == '(')
      {
        strcpy(list[lex_index].name, "(");
        list[lex_index].type = lparensym;
        lex_index++;
      }
      else if(input[i] == ')')
      {
        strcpy(list[lex_index].name, ")");
        list[lex_index].type = rparensym;
        lex_index++;
      }
      else
      {
        printlexerror(1);
				return NULL;
      }
		
    }
	}

	if (printFlag)
		printtokens();
	
  list[lex_index++].type = -1;
	return list;
}

void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
			case oddsym:
				printf("%11s\t%d", "odd", oddsym);
				break;
			case eqlsym:
				printf("%11s\t%d", "=", eqlsym);
				break;
			case neqsym:
				printf("%11s\t%d", "!=", neqsym);
				break;
			case lsssym:
				printf("%11s\t%d", "<", lsssym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case modsym:
				printf("%11s\t%d", "%", modsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case divsym:
				printf("%11s\t%d", "/", divsym);
				break;
			case addsym:
				printf("%11s\t%d", "+", addsym);
				break;
			case subsym:
				printf("%11s\t%d", "-", subsym);
				break;
			case lparensym:
				printf("%11s\t%d", "(", lparensym);
				break;
			case rparensym:
				printf("%11s\t%d", ")", rparensym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case assignsym:
				printf("%11s\t%d", ":=", assignsym);
				break;
			case odsym:
				printf("%11s\t%d", "od", odsym);
				break;
			case whensym:
				printf("%11s\t%d", "when", whensym);
				break;
			case elsedosym:
				printf("%11s\t%d", "elsedo", elsedosym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case constsym:
				printf("%11s\t%d", "const", constsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
		}
		printf("\n");
	}
	printf("\n");
	printf("Token List:\n");
	for (i = 0; i < lex_index; i++)
	{
		if (list[i].type == numbersym)
			printf("%d %d ", numbersym, list[i].value);
		else if (list[i].type == identsym)
			printf("%d %s ", identsym, list[i].name);
		else
			printf("%d ", list[i].type);
	}
	printf("\n");
}

void printlexerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Excessive Number Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Excessive Identifier Length\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");
	
	free(list);
	return;
}