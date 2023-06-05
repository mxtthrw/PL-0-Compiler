/*
  Authors: Nathan Manierre and Matthew Dahbar

	This is the skeleton parser.c for the UCF Fall 2021 Systems Software 
	Project, HW4. Add whatever you want, but make sure to leave the
	print functions as they are to make sure you get your grade as
	quickly as possible. There is a line commented out just before
	the return statement in parse(). Make sure to uncomment it
	after you implement the function, your vm.c may malfunction
	without it.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 1000
#define MAX_SYMBOL_COUNT 100

#define JMP 7, 0
#define INC 6, 0
#define RTN 2, 0, 0
#define JPC 8, 0
#define READ 9, 0, 2
#define WRITE 9, 0, 1
#define HALT 9, 0, 3
#define NEG 2, 0, 1
#define ADD 2, 0, 2
#define SUB 2, 0, 3
#define MUL 2, 0, 4
#define DIV 2, 0, 5
#define ODD 2, 0, 6
#define MOD 2, 0, 7
#define EQL 2, 0, 8
#define NEQ 2, 0, 9
#define LSS 2, 0, 10
#define LEQ 2, 0, 11
#define GTR 2, 0, 12
#define GEQ 2, 0, 13
#define LIT 1, 0

instruction *code;
int cIndex;
symbol *table;
int tIndex;
int listIndex = 0;
int stopFlag = 0;

void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();
void PROGRAM(lexeme *list, int level);
void BLOCK(lexeme *list, int level);
void MARK(lexeme *list, int level);
int MULTIPLEDECLARATIONCHECK(lexeme *list, int level);
void CONST_DECLARATION(lexeme *list, int level);
int VAR_DECLARATION(lexeme *list, int level);
void PROCEDURE_DECLARATION(lexeme *list, int level);
void STATEMENT(lexeme *list, int level);
void CONDITION(lexeme *list, int level);
void EXPRESSION(lexeme *list, int level);
void TERM(lexeme *list, int level);
void FACTOR(lexeme *list, int level);
int FINDSYMBOL(lexeme *list, int num, int level);

void MARK(lexeme *list, int level)
{
  int i = tIndex; 
  while (i >= 0)
  {
    if (table[i].mark == 0)   // marked value is 1
    {    
      if (table[i].level == level)
        table[i].mark = 1;
    }
    i--;
  }
}

int MULTIPLEDECLARATIONCHECK (lexeme *list, int level)
{
	for (int i = 0; i < MAX_SYMBOL_COUNT; i++)
	{
		if (strcmp(table[i].name, list[i].name) == 0 && strcmp(list[i].name, " ") == 1)
    {
      if (table[i].mark == 0)
      {
        if (table[i].level == level)
        {
          return i;
        }
      }
		}		
	}
  return -1;
}

int FINDSYMBOL(lexeme *list, int kind, int level) 
{
  for (int i = tIndex - 1; i >= 0; i--)
  {
    if (strcmp(list[listIndex].name, table[i].name) == 0)
    {
      if ((table[i].kind == kind) && (table[i].mark == 0) && (table[i].level <= level))
      {
        return i;
      }
    }
  }
  return -1;
}

void PROGRAM(lexeme *list, int level)
{
	emit(JMP, 0);
	addToSymbolTable(3, "main", 0, 0, 0, 0);
	level = -1;

	BLOCK(list, level);
	if (stopFlag == 1)
    return;

	if (list[listIndex].type != periodsym)
	{
		printparseerror(1);
    
    if (stopFlag == 1)
      return;
	}

	emit(HALT);
	for (int line = 0; line < cIndex; line++)
	{
		if (code[line].opcode == 5)
		{
			code[line].m = table[code[line].m].addr;
		}
	}	
  code[0].m = table[0].addr;
}

void BLOCK(lexeme *list, int level)
{
	level++;
	int procedure_idx = tIndex - 1;
	CONST_DECLARATION(list, level);
	if (stopFlag == 1)
    return;

	int x = VAR_DECLARATION(list, level);
	if (stopFlag == 1)
    return;
	
	PROCEDURE_DECLARATION(list, level);
	if (stopFlag == 1)
    return;

	table[procedure_idx].addr = cIndex * 3;
	
	if (level == 0)
	{
		emit(INC, x);
	}
	else
	{
		emit(INC, x + 4);
	}
	STATEMENT(list, level);
	if (stopFlag == 1)
    return;
	MARK(list, level);
	if (stopFlag == 1)
    return;
	level--;
}

void CONST_DECLARATION(lexeme *list, int level)
{
	if (list[listIndex].type == constsym)
	{
		do {
			// get next token
			listIndex++; 
			
			if (list[listIndex].type != identsym)
      {
				// error
        printparseerror(2);
        
        if (stopFlag == 1)
          return;
      }
			
			int symidx = MULTIPLEDECLARATIONCHECK(list, level);
			if (stopFlag == 1)
    		return;

			if (symidx != -1)
      {
				printparseerror(18);  
        
        if (stopFlag == 1)
          return;
      }
			// save ident name
      char savedname[12];
			strcpy(savedname, list[listIndex].name);

			// get next token
			listIndex++;

			if (list[listIndex].type != assignsym)
      {
				printparseerror(2);  

        if (stopFlag == 1)
          return;
      }
			// get next token
			listIndex++;

			if (list[listIndex].type != numbersym)
      {
				printparseerror(18);  
        
        if (stopFlag == 1)
          return;
      }
			addToSymbolTable(1, savedname, list[listIndex].value, level, 0, 0);
			// get next token 
			listIndex++;

		} while (list[listIndex].type == commasym);
		if (list[listIndex].type != semicolonsym)
		{
			if (list[listIndex].type == identsym)
      {
				printparseerror(13);  

        if (stopFlag == 1)
          return;
      }
			else
      {
				printparseerror(14);  

        if (stopFlag == 1)
          return;
      }
		}
		// get next 
		listIndex++;
	}
}

int VAR_DECLARATION(lexeme *list, int level)
{
	int numVars = 0;

	if (list[listIndex].type == varsym)
	{
		do {
		numVars++;
		// get next token
		listIndex++;
		
		if (list[listIndex].type != identsym)
    {
			printparseerror(3);  
      
      if (stopFlag == 1)
       return 0;
    }
    int symidx = MULTIPLEDECLARATIONCHECK(list, level);
		if (stopFlag == 1)
      return 0;

    if (symidx != -1)
    {
      printparseerror(18);
      
      if (stopFlag == 1)
        return 0;
    } 
    
		if (level == 0)
			addToSymbolTable (2, list[listIndex].name, 0, level, numVars-1, 0);
		else
    {
			addToSymbolTable (2, list[listIndex].name, 0, level, numVars+3, 0);
    }
		// get next token
		listIndex++;
		} while (list[listIndex].type == commasym);

		if (list[listIndex].type != semicolonsym)
		{
			if (list[listIndex].type == identsym)
      {
				printparseerror(13);  
        if (stopFlag == 1)
          return 0;
      }
			else
      {
				printparseerror(14);  
        
        if (stopFlag == 1)
          return 0;
      }
		}
		// get next token
		listIndex++;
	}
	return numVars;
}

void FACTOR(lexeme *list, int level)
{
	if (list[listIndex].type == identsym)
	{
		int symIdx_var = FINDSYMBOL(list, 2, level);
		int symIdx_const = FINDSYMBOL(list, 1, level);
		if ((symIdx_var == -1) && (symIdx_const == -1))
		{
			if ((FINDSYMBOL(list, 3, level)) != -1)
			{
				if (stopFlag == 1)
    			return;
					
				printparseerror(11);  
        
        if (stopFlag == 1)
          return;
			}
			else
			{
				printparseerror(19);  

        if (stopFlag == 1)
          return;
			}
		}
		if (symIdx_var == -1)
		{
			emit(1, 0, table[symIdx_const].val); //LIT
      listIndex++;
		}
		else if ((symIdx_const == -1) || (table[symIdx_var].level > table[symIdx_const].level))
		{
			emit(3, level-table[symIdx_var].level, table[symIdx_var].addr); //LOD
      listIndex++;
		}
		else
		{
			emit(LIT, table[symIdx_const].val);
			// get next token;
			listIndex++;
			
		}
	}
	else if (list[listIndex].type == numbersym)
	{
		emit(LIT, list[listIndex].value);
		// get next token 
		listIndex++;
	}
	else if (list[listIndex].type == lparensym)
	{
		// get next token
		listIndex++;
		EXPRESSION(list, level);
		if (stopFlag == 1)
    	return;

		if (list[listIndex].type != rparensym)
		{
			printparseerror(12);  

      if (stopFlag == 1)
        return;
		}
		// get next token
		listIndex++;
	}
	else
	{
		printparseerror(19);   

    if (stopFlag == 1)
      return;
	}
}

void TERM(lexeme *list, int level)
{
	FACTOR(list, level);
  // tip.txt still running here
  if (stopFlag == 1)
    return;
	
	while (list[listIndex].type == multsym || list[listIndex].type == divsym || list[listIndex].type == modsym)
	{
		if (list[listIndex].type == multsym)
		{
			// get next token
			listIndex++;

			FACTOR(list, level);
      if (stopFlag == 1)
        return;

			emit(MUL);
		}
		else if (list[listIndex].type == divsym)
		{
			// get next token
			listIndex++;

			FACTOR(list, level);
      if (stopFlag == 1)
        return;

			emit(DIV);
		}
		else
		{
			// get next token
			listIndex++;

			FACTOR(list, level);
      if (stopFlag == 1)
        return;

			emit(MOD);
		}
	}	
}

void EXPRESSION(lexeme *list, int level)
{
	if (list[listIndex].type == subsym)
	{
		// get next token
		listIndex++;
		TERM(list, level);
    
    if (stopFlag == 1)
      return;

		emit(NEG);
		while ((list[listIndex].type == addsym ) || (list[listIndex].type == subsym) )
		{
			if (list[listIndex].type == addsym)
			{
				// get next token
				listIndex++;
				TERM(list, level);

        if (stopFlag == 1)
          return;

        emit(ADD);
			}
      else
      {
        listIndex++;
        TERM(list, level);

        if (stopFlag == 1)
          return;

        emit(SUB);
      }
		}	
	}
  else
  {
    if (list[listIndex].type == addsym)
    {
      listIndex++;
    }
    TERM(list, level);

    if (stopFlag == 1)
      return;

    while (list[listIndex].type == addsym || list[listIndex].type == subsym)
    {
      if (list[listIndex].type == addsym)
      {
        listIndex++;
        TERM(list, level);

        if (stopFlag == 1)
          return;

        emit(ADD);
      }
      else
      {
        listIndex++;
        TERM(list, level);

        if (stopFlag == 1)
          return;

        emit(SUB);
      }
    }
  }
  if ((list[listIndex].type == lparensym) || (list[listIndex].type == identsym) || (list[listIndex].type == numbersym) || (list[listIndex].type == oddsym))
  {
    printparseerror(17);

    if (stopFlag == 1)
      return;
  }
}

void CONDITION(lexeme *list, int level)
{
	if (list[listIndex].type == oddsym)
	{
		// get next token
		listIndex++;

		EXPRESSION(list, level);

    if (stopFlag == 1)
      return;

		emit(ODD);
	}
	else
	{
		EXPRESSION(list, level);

    if (stopFlag == 1)
      return;

		if (list[listIndex].type == eqlsym)
		{
			// get next token
			listIndex++;

			EXPRESSION(list, level);

      if (stopFlag == 1)
        return;

			emit(EQL);
		}	
		else if (list[listIndex].type == neqsym)
		{
			// get next token
			listIndex++;

			EXPRESSION(list, level);

      if (stopFlag == 1)
        return;

			emit(NEQ);	
		}	
		else if (list[listIndex].type == lsssym)
		{
			// get next token
			listIndex++;

			EXPRESSION(list, level);

      if (stopFlag == 1)
        return;

			emit(LSS);
		}	
		else if (list[listIndex].type == leqsym)
		{
			// get next token
			listIndex++;

			EXPRESSION(list, level);

      if (stopFlag == 1)
        return;

			emit(LEQ);
		}
		else if (list[listIndex].type == gtrsym)
		{
			// get next token
			listIndex++;

			EXPRESSION(list, level);

      if (stopFlag == 1)
        return;
      
			emit(GTR);
		}
		else if (list[listIndex].type == geqsym)
		{
			// get next token
			listIndex++;

			EXPRESSION(list, level);

      if (stopFlag == 1)
        return;

			emit(GEQ);
		}
		else
		{
			printparseerror(10);  

      if (stopFlag == 1)
        return;
		}
	}
}

void STATEMENT(lexeme *list, int level)
{
	if (list[listIndex].type == identsym)
	{
		int symIdx = FINDSYMBOL(list, 2, level);
		
		if (symIdx == -1)
		{
			if (FINDSYMBOL(list, 1, level) != FINDSYMBOL(list, 3, level))
			{
				if (stopFlag == 1)
    			return;

				printparseerror(6);  

        if (stopFlag == 1)
          return;
			}	
				
			else
			{
				printparseerror(19);  

        if (stopFlag == 1)
          return;
			}
				
		}
		// get next token
		listIndex++;

		if (list[listIndex].type != assignsym)
    {
			printparseerror(5);  

      if (stopFlag == 1)
        return;
    }
		// get next token
		listIndex++;

		EXPRESSION(list, level);
		if (stopFlag == 1)
    	return;

		emit(4, level - table[symIdx].level, table[symIdx].addr); //STO
		return;
	}
	
	if (list[listIndex].type == dosym)
	{
		do {
			// get next token
			listIndex++;

			STATEMENT(list, level);

      if (stopFlag == 1)
        return;

		} while (list[listIndex].type == semicolonsym);

		if (list[listIndex].type != odsym)
		{
			if ((list[listIndex].type == identsym) || (list[listIndex].type == dosym) || (list[listIndex].type == whensym) || (list[listIndex].type == whilesym) || (list[listIndex].type == readsym) || (list[listIndex].type == writesym) || (list[listIndex].type == callsym))
      {
				printparseerror(15);  

        if (stopFlag == 1)
          return;
      }

			else
      {
				printparseerror(16);  

        if (stopFlag == 1)
          return;
      }
		}

		// get next token
		listIndex++;

		return;
	}

	if (list[listIndex].type == whensym)
	{
		// get next token
		listIndex++;

		CONDITION(list, level);

    if (stopFlag == 1)
      return;

		int jpcIdx = cIndex;
		emit(JPC, 0);

		if (list[listIndex].type != dosym)
    {
			printparseerror(8);  

      if (stopFlag == 1)
        return;
    }
		// get next token
		listIndex++;

		STATEMENT(list, level);

    if (stopFlag == 1)
      return;

		if (list[listIndex].type == elsedosym)
		{
			int jmpIdx = cIndex;
			emit(JMP, 0);
			code[jpcIdx].m = cIndex * 3;
      listIndex++;
			STATEMENT(list, level);

      if (stopFlag == 1)
        return;

			code[jmpIdx].m = cIndex * 3;
		}
		else
		{
			code[jpcIdx].m = cIndex * 3;
		}
		return;
	}

	if (list[listIndex].type == whilesym)
	{
		// get next token
		listIndex++;

		int loopIdx = cIndex;
		CONDITION(list, level);

    if (stopFlag == 1)
      return;
		
		if (list[listIndex].type != dosym)
    {
			printparseerror(9);  
      
      if (stopFlag == 1)
        return;
    }
		// get next token
		listIndex++;
		
		int jpcIdx = cIndex;
		emit(JPC, 0);
		STATEMENT(list, level);

    if (stopFlag == 1)
      return;

		emit(JMP, loopIdx * 3); //JMP
		code[jpcIdx].m = cIndex * 3;
		return;
	}

	if (list[listIndex].type == readsym)
	{
		// get next token
		listIndex++;
		
		if (list[listIndex].type != identsym)
    {
			printparseerror(19); // maybe 19?
      
      if (stopFlag == 1)
        return;
    }
		int symIdx = FINDSYMBOL(list, 2, level);
		if (stopFlag == 1)
    	return;
		
		if (symIdx == -1)
		{
			if (FINDSYMBOL(list, 1, level) != FINDSYMBOL(list, 3, level))
      {
				if (stopFlag == 1)
    			return;

				printparseerror(7);  
        
        if (stopFlag == 1)
          return;
      }
			else
      {
				printparseerror(19);  
        
        if (stopFlag == 1)
          return;
      }
		}
		// get next token
		listIndex++;

		emit(READ);
		emit(4, level - table[symIdx].level, table[symIdx].addr);//STO
		return;
	}

	if (list[listIndex].type == writesym)
	{
		// get next token
		listIndex++;

		EXPRESSION(list, level);

    if (stopFlag == 1)
      return;

		emit(WRITE);
		return;
	}

	if (list[listIndex].type == callsym)
	{
		// get next token
		listIndex++;

		int symIdx = FINDSYMBOL(list, 3, level);
		
		if (symIdx == -1)
		{
			if (FINDSYMBOL(list, 1, level) != FINDSYMBOL(list, 2, level))
      {
				if (stopFlag == 1)
    			return;

				printparseerror(7);  

        if (stopFlag == 1)
          return;
      }
			else
      {
				printparseerror(19);  

        if (stopFlag == 1)
          return;
      }
		}
		// get next token
		listIndex++;

		emit(5, level - table[symIdx].level, symIdx);
	}
}

void PROCEDURE_DECLARATION(lexeme *list, int level)
{
	while (list[listIndex].type == procsym)
	{
		// get next token;
		listIndex++;

		if (list[listIndex].type != identsym)
		{
			printparseerror(2);  

      if (stopFlag == 1)
        return;
		}

		int symidx = MULTIPLEDECLARATIONCHECK(list, level);
		if (stopFlag == 1)
			return;

		if (symidx != -1)
		{
			printparseerror(18);  

      if (stopFlag == 1)
        return;
		}
		addToSymbolTable(3, list[listIndex].name, 0, level, 0, 0);
		// get next token
		listIndex++;

		if (list[listIndex].type != semicolonsym)
		{
			printparseerror(4);  

      if (stopFlag == 1)
        return;
		}
		// get next token
		listIndex++;
    
		BLOCK(list, level);

    if (stopFlag == 1)
      return;

		if (list[listIndex].type != semicolonsym)
		{
			printparseerror(18);  

      if (stopFlag == 1)
        return;
		}
		
		// get next token
		listIndex++;

		emit(RTN);
	}
}

instruction *parse(lexeme *list, int printTable, int printCode)
{
  int level = 0;
	code = NULL;

  code = malloc(sizeof(instruction)* MAX_CODE_LENGTH);
  table = malloc(sizeof(symbol)* MAX_SYMBOL_COUNT);
	
	// your parsing code here
  PROGRAM(list, level);

  if (stopFlag == 1)
    return NULL;
	
	if (printTable)
		printsymboltable();
	if (printCode)
		printassemblycode();
	
	code[cIndex].opcode = -1;
	return code;
}

void emit(int opname, int level, int mvalue)
{
	code[cIndex].opcode = opname;
	code[cIndex].l = level;
	code[cIndex].m = mvalue;
	cIndex++;
}

void addToSymbolTable(int k, char n[], int v, int l, int a, int m)
{
	table[tIndex].kind = k;
	strcpy(table[tIndex].name, n);
	table[tIndex].val = v;
	table[tIndex].level = l;
	table[tIndex].addr = a;
	table[tIndex].mark = m;
	tIndex++;
}

void printparseerror(int err_code)
{
	switch (err_code)
	{
		case 1:
			printf("Parser Error: Program must be closed by a period\n");
			break;
		case 2:
			printf("Parser Error: Constant declarations should follow the pattern 'ident := number {, ident := number}'\n");
			break;
		case 3:
			printf("Parser Error: Variable declarations should follow the pattern 'ident {, ident}'\n");
			break;
		case 4:
			printf("Parser Error: Procedure declarations should follow the pattern 'ident ;'\n");
			break;
		case 5:
			printf("Parser Error: Variables must be assigned using :=\n");
			break;
		case 6:
			printf("Parser Error: Only variables may be assigned to or read\n");
			break;
		case 7:
			printf("Parser Error: call must be followed by a procedure identifier\n");
			break;
		case 8:
			printf("Parser Error: when must be followed by do\n");
			break;
		case 9:
			printf("Parser Error: while must be followed by do\n");
			break;
		case 10:
			printf("Parser Error: Relational operator missing from condition\n");
			break;
		case 11:
			printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, constants, and variables\n");
			break;
		case 12:
			printf("Parser Error: ( must be followed by )\n");
			break;
		case 13:
			printf("Parser Error: Multiple symbols in variable and constant declarations must be separated by commas\n");
			break;
		case 14:
			printf("Parser Error: Symbol declarations should close with a semicolon\n");
			break;
		case 15:
			printf("Parser Error: Statements within do-od must be separated by a semicolon\n");
			break;
		case 16:
			printf("Parser Error: do must be followed by od\n");
			break;
		case 17:
			printf("Parser Error: Bad arithmetic\n");
			break;
		case 18:
			printf("Parser Error: Confliciting symbol declarations\n");
			break;
		case 19:
			printf("Parser Error: Undeclared identifier\n");
			break;
		default:
			printf("Implementation Error: unrecognized error code\n");
			break;
	}
	
	free(code);
	free(table);
  stopFlag = 1;
}

void printsymboltable()
{
	int i;
	printf("Symbol Table:\n");
	printf("Kind | Name        | Value | Level | Address | Mark\n");
	printf("---------------------------------------------------\n");
	for (i = 0; i < tIndex; i++)
		printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr, table[i].mark); 
	
	free(table);
	table = NULL;
}

void printassemblycode()
{
	int i;
	printf("Line\tOP Code\tOP Name\tL\tM\n");
	for (i = 0; i < cIndex; i++)
	{
		printf("%d\t", i);
		printf("%d\t", code[i].opcode);
		switch (code[i].opcode)
		{
			case 1:
				printf("LIT\t");
				break;
			case 2:
				switch (code[i].m)
				{
					case 0:
						printf("RTN\t");
						break;
					case 1:
						printf("NEG\t");
						break;
					case 2:
						printf("ADD\t");
						break;
					case 3:
						printf("SUB\t");
						break;
					case 4:
						printf("MUL\t");
						break;
					case 5:
						printf("DIV\t");
						break;
					case 6:
						printf("ODD\t");
						break;
					case 7:
						printf("MOD\t");
						break;
					case 8:
						printf("EQL\t");
						break;
					case 9:
						printf("NEQ\t");
						break;
					case 10:
						printf("LSS\t");
						break;
					case 11:
						printf("LEQ\t");
						break;
					case 12:
						printf("GTR\t");
						break;
					case 13:
						printf("GEQ\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			case 3:
				printf("LOD\t");
				break;
			case 4:
				printf("STO\t");
				break;
			case 5:
				printf("CAL\t");
				break;
			case 6:
				printf("INC\t");
				break;
			case 7:
				printf("JMP\t");
				break;
			case 8:
				printf("JPC\t");
				break;
			case 9:
				switch (code[i].m)
				{
					case 1:
						printf("WRT\t");
						break;
					case 2:
						printf("RED\t");
						break;
					case 3:
						printf("HAL\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			default:
				printf("err\t");
				break;
		}
		printf("%d\t%d\n", code[i].l, code[i].m);
	}
	if (table != NULL)
		free(table);
}