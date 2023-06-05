/*
  Authors: Nathan Manierre and Matthew Dahbar

	This is the skeleton vm.c for the UCF Fall 2021 Systems Software 
	Project, HW4. Add whatever you want, but make sure to leave the
	print functions as they are to make sure you get your grade as
	quickly as possible. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#define MAX_PAS_LENGTH 3000

int IC = 0;
int GP;		// Global Pointer – Points to DATA segment
int DP;		// Data Pointer – To access variables in Main
int FREE;	// FREE points to Heap
int BP;		// Points to base of DATA or activation records
int PC = 0; // Stack pointer – Points to top of stack
int SP = MAX_PAS_LENGTH;

int pas[MAX_PAS_LENGTH];
int IR[3]; // Instruction register


void print_execution(int line, char *opname, int *IR, int PC, int BP, int SP, int DP, int *pas, int GP)
{
	int i;
	// print out instruction and registers
	printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, opname, IR[1], IR[2], PC, BP, SP, DP);
	
	// print data section
	for (i = GP; i <= DP; i++)
		printf("%d ", pas[i]);
	printf("\n");
	
	// print stack
	printf("\tstack : ");
	for (i = MAX_PAS_LENGTH - 1; i >= SP; i--)
		printf("%d ", pas[i]);
	printf("\n");
}

int base(int L, int BP, int *pas)
{
	int ctr = L;
	int rtn = BP;
	while (ctr > 0)
	{
		rtn = pas[rtn-1];
		ctr--;
	}
	return rtn;
}

void execute_program(instruction *code, int printFlag)
{
	// variables
	int *pas = calloc(MAX_PAS_LENGTH, sizeof(int));
	int IC, line;
  char *opname;
	
	// read in program
	IC = 0;
	line = 0;

	while (code[line].opcode != -1)
	{
		pas[IC] = code[line].opcode;
		IC++;
		pas[IC] = code[line].l;
		IC++;
		pas[IC] = code[line].m;
		IC++;
		line++;
	}

  GP = IC;		// Global Pointer – Points to DATA segment
	DP = IC - 1;	// Data Pointer – To access variables in Main
	FREE = IC + 40; // FREE points to Heap
	BP = IC;		// Points to base of DATA or activation records

  printf("\t\t\t\tPC\tBP\tSP\tDP\tdata\n");
	printf("Initial values:\t\t\t%d\t%d\t%d\t%d\n", PC, BP, SP, DP);

	int halt = 0;
	int line_count = 0;
	while (halt == 0)
	{
		line_count = PC / 3;
		IR[0] = pas[PC];
		IR[1] = pas[PC + 1];
		IR[2] = pas[PC + 2];
		PC = PC + 3;

		int OP = IR[0]; // First number read in from line
		int L = IR[1];	// Second number read in from line
		int M = IR[2];	// Third number read in from line

		// we will have 24 switch cases (some nested)
		// follow the logic in the pseudo code
		switch (OP)
		{
		case 1: // LIT
			if (BP == GP)
			{
				DP = DP + 1;
				pas[DP] = M;
			}
			else
			{
				SP = SP - 1;
				pas[SP] = M;
			}
			opname = "LIT";
			print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
			break;

		case 2: // OPR
			// this case will have nested switch cases
			switch (M)
			{
			case 0: // RTN
				SP = BP + 1;
				BP = pas[SP - 3];
				PC = pas[SP - 4];
				opname = "RTN";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 1: // NEG
				if (BP == GP)
				{
					pas[DP] = -1 * pas[DP];
				}
				else
				{
					pas[SP] = -1 * pas[SP];
				}
				opname = "NEG";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 2: // ADD
				if (BP == GP)
				{
					DP = DP - 1;
					pas[DP] = pas[DP] + pas[DP + 1];
				}
				else
				{
					SP = SP + 1;
					pas[SP] = pas[SP] + pas[SP - 1];
				}
				opname = "ADD";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 3: // SUB
				if (BP == GP)
				{
					DP = DP - 1;
					pas[DP] = pas[DP] - pas[DP + 1];
				}
				else
				{
					SP = SP + 1;
					pas[SP] = pas[SP] - pas[SP - 1];
				}
				opname = "SUB";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 4: // MUL
				if (BP == GP)
				{
					DP = DP - 1;
					pas[DP] = pas[DP] * pas[DP + 1];
				}
				else
				{
					SP = SP + 1;
					pas[SP] = pas[SP] * pas[SP - 1];
				}
				opname = "MUL";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 5: // DIV
				if (BP == GP)
				{
					DP = DP - 1;
					pas[DP] = pas[DP] / pas[DP + 1];
				}
				else
				{
					SP = SP + 1;
					pas[SP] = pas[SP] / pas[SP - 1];
				}
				opname = "DIV";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 6: // ODD
				if (BP == GP)
				{
					pas[DP] = pas[DP] % 2;
				}
				else
				{
					pas[SP] = pas[SP] % 2;
				}
				opname = "ODD";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 7: // MOD
				if (BP == GP)
				{
					DP = DP - 1;
					pas[DP] = pas[DP] % pas[DP + 1];
				}
				else
				{
					SP = SP + 1;
					pas[SP] = pas[SP] % pas[SP - 1];
				}
				opname = "MOD";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 8: // EQL
				if (BP == GP)
				{
					DP = DP - 1;
					pas[DP] = (pas[DP] == pas[DP + 1]);
				}
				else
				{
					SP = SP + 1;
					pas[SP] = (pas[SP] == pas[SP - 1]);
				}
				opname = "EQL";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 9: // NEQ
				if (BP == GP)
				{
					DP = DP - 1;
					pas[DP] = (pas[DP] != pas[DP + 1]);
				}
				else
				{
					SP = SP + 1;
					pas[SP] = (pas[SP] != pas[SP - 1]);
				}
				opname = "NEQ";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 10: // LSS
				if (BP == GP)
				{
					DP = DP - 1;
					pas[DP] = (pas[DP] < pas[DP + 1]);
				}
				else
				{
					SP = SP + 1;
					pas[SP] = (pas[SP] < pas[SP - 1]);
				}
				opname = "LSS";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 11: // LEQ
				if (BP == GP)
				{
					DP = DP - 1;
					pas[DP] = (pas[DP] <= pas[DP + 1]);
				}
				else
				{
					SP = SP + 1;
					pas[SP] = (pas[SP] <= pas[SP - 1]);
				}
				opname = "LEQ";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 12: // GTR
				if (BP == GP)
				{
					DP = DP - 1;
					pas[DP] = (pas[DP] > pas[DP + 1]);
				}
				else
				{
					SP = SP + 1;
					pas[SP] = (pas[SP] > pas[SP - 1]);
				}
				opname = "GTR";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 13: // GEQ
				if (BP == GP)
				{
					DP = DP - 1;
					pas[DP] = (pas[DP] >= pas[DP + 1]);
				}
				else
				{
					SP = SP + 1;
					pas[SP] = (pas[SP] >= pas[SP - 1]);
				}
				opname = "GEQ";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;
			}
			break;

		case 3: // LOD
			if (BP == GP)
			{
				DP = DP + 1;
				pas[DP] = pas[GP + M];
			}
			else
			{
				if (base(L, BP, pas) == GP)
				{
					SP = SP - 1;
					pas[SP] = pas[GP + M];
				}
				else
				{
					SP = SP - 1;
					pas[SP] = pas[base(L, BP, pas) - M];
				}
			}
			opname = "LOD";
			print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
			break;

		case 4: // STO
			if (BP == GP)
			{
				pas[GP + M] = pas[DP];
				DP = DP - 1;
			}
			else
			{
				if (base(L, BP, pas) == GP)
				{
					pas[GP + M] = pas[SP];
					SP = SP + 1;
				}
				else
				{
					pas[base(L, BP, pas) - M] = pas[SP];
					SP = SP + 1;
				}
			}
			opname = "STO";
			print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
			break;

		case 5:					   // CAL
      pas[SP - 1] = 0;
			pas[SP - 2] = base(L, BP, pas); // static link (SL)
			pas[SP - 3] = BP;	   // dynamic link (DL)
			pas[SP - 4] = PC;	   // return address
			BP = SP - 1;
			PC = M;
			opname = "CAL";
			print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
			break;

		case 6: // INC
			if (BP == GP)
			{
				DP = DP + M;
			}
			else
			{
				SP = SP - M;
			}
			opname = "INC";
			print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
			break;

		case 7: // JMP
			PC = M;
			opname = "JMP";
			print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
			break;

		case 8: // JPC
			if (BP == GP)
			{
				if (pas[DP] == 0)
				{
					PC = M;
				}
				DP = DP - 1;
			}
			else
			{
				if (pas[SP] == 0)
				{
					PC = M;
				}
				SP = SP + 1;
			}
			opname = "JPC";
			print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
			break;

		case 9: // SYS
				// this case will have nested switch cases
			switch (M)
			{
			case 1:
				// Prints value at the top of stack
				if (BP == GP)
				{
					printf("Top of Stack Value: ");
					printf("%d\n", pas[DP]);
					DP = DP - 1;
				}
				else
				{
					printf("Top of Stack Value: ");
					printf("%d\n", pas[SP]);
					SP = SP + 1;
				}
				opname = "SYS";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 2:
				// Asks for integer input and scans
				if (BP == GP)
				{
					DP = DP + 1;
					printf("Please Enter an Integer: ");
					scanf("%d", &pas[DP]);
				}
				else
				{
					SP = SP - 1;
					printf("Please Enter an Integer: ");
					scanf("%d", &pas[SP]);
				}
				opname = "SYS";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				break;

			case 3:
				// * end of program *
				opname = "SYS";
				print_execution(line_count, opname, IR, PC, BP, SP, DP, pas, GP);
				halt = 1;
				break;
			}
			break;

		default:
			break;
		}
	}

	
	// this will print the header in the output, make sure you put your
	//		execution loop after it
	if (printFlag)
	{
		// printf("\t\t\t\tPC\tBP\tSP\tDP\tdata\n");
		// printf("Initial values:\t\t\t%d\t%d\t%d\t%d\n", PC, BP, SP, DP);
	}
	
	free(pas);
}
