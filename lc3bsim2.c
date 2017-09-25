/*
    REFER TO THE SUBMISSION INSTRUCTION FOR DETAILS

    Name 1: Full name of the first partner
    Name 2: Full name of the second partner
    UTEID 1: UT EID of the first partner
    UTEID 2: UT EID of the second partner
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A
*/

#define WORDS_IN_MEM    0x08000
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */


typedef struct System_Latches_Struct{

  int PC,		/* program counter */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P;		/* p condition bit */
  int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {
  printf("----------------LC-3b ISIM Help-----------------------\n");
  printf("go               -  run program to completion         \n");
  printf("run n            -  execute program for n instructions\n");
  printf("mdump low high   -  dump memory from low to high      \n");
  printf("rdump            -  dump the register & bus values    \n");
  printf("?                -  display this help menu            \n");
  printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {

  process_instruction();
  CURRENT_LATCHES = NEXT_LATCHES;
  INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {
  int i;

  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++) {
    if (CURRENT_LATCHES.PC == 0x0000) {
	    RUN_BIT = FALSE;
	    printf("Simulator halted\n\n");
	    break;
    }
    cycle();
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go() {
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating...\n\n");
  while (CURRENT_LATCHES.PC != 0x0000)
    cycle();
  RUN_BIT = FALSE;
  printf("Simulator halted\n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {
  int address; /* this is a byte address */

  printf("\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    printf("  0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    fprintf(dumpsim_file, " 0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {
  int k;

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  printf("Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    fprintf(dumpsim_file, "%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {
  char buffer[20];
  int start, stop, cycles;

  printf("LC-3b-SIM> ");

  scanf("%s", buffer);
  printf("\n");

  switch(buffer[0]) {
  case 'G':
  case 'g':
    go();
    break;

  case 'M':
  case 'm':
    scanf("%i %i", &start, &stop);
    mdump(dumpsim_file, start, stop);
    break;

  case '?':
    help();
    break;
  case 'Q':
  case 'q':
    printf("Bye.\n");
    exit(0);

  case 'R':
  case 'r':
    if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
    else {
	    scanf("%d", &cycles);
	    run(cycles);
    }
    break;

  default:
    printf("Invalid Command\n");
    break;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {
  int i;

  for (i=0; i < WORDS_IN_MEM; i++) {
    MEMORY[i][0] = 0;
    MEMORY[i][1] = 0;
  }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {
  FILE * prog;
  int ii, word, program_base;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL) {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  /* Read in the program. */
  if (fscanf(prog, "%x\n", &word) != EOF)
    program_base = word >> 1;
  else {
    printf("Error: Program file is empty\n");
    exit(-1);
  }

  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF) {
    /* Make sure it fits. */
    if (program_base + ii >= WORDS_IN_MEM) {
	    printf("Error: Program file %s is too long to fit in memory. %x\n",
             program_filename, ii);
	    exit(-1);
    }

    /* Write the word to memory array. */
    MEMORY[program_base + ii][0] = word & 0x00FF;
    MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
    ii++;
  }

  if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

  printf("Read %d words from program into memory.\n\n", ii);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files) {
  int i;

  init_memory();
  for ( i = 0; i < num_prog_files; i++ ) {
    load_program(program_filename);
    while(*program_filename++ != '\0');
  }
  CURRENT_LATCHES.Z = 1;
  NEXT_LATCHES = CURRENT_LATCHES;

  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {
  FILE * dumpsim_file;

  /* Error Checking */
  if (argc < 2) {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("LC-3b Simulator\n\n");

  initialize(argv[1], argc - 1);

  if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  //while (1)
    //get_command(dumpsim_file);
	  go();
  	  mdump(dumpsim_file, 0x3000, 0x3100);
  	  rdump(dumpsim_file);
  	  printf("Bye.\n");
  	  exit(0);

}

/***************************************************************/
/* Do not modify the above code.
   You are allowed to use the following global variables in your
   code. These are defined above.

   MEMORY

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */

/***************************************************************/


#define DEBUG
#define NUM_OF_OPCODES 16
void isa_add(int word);
void isa_and(int word);
void isa_br(int word);
void isa_jmp(int word);
void isa_jsr(int word);
void isa_ldb(int word);
void isa_ldw(int word);
void isa_lea(int word);
void isa_rti(int word);
void isa_shf(int word);
void isa_stb(int word);
void isa_stw(int word);
void isa_trap(int word);
void isa_xor(int word);
void isa_not_used(int word);
int sext(int number, int size);
void setCC(int result);

void (*isa_ptr[NUM_OF_OPCODES]) (int) =
		{	isa_br, /*0000*/
			isa_add,
			isa_ldb,
			isa_stb,
			isa_jsr, /*0100*/
			isa_and,
			isa_ldw,
			isa_stw,
			isa_rti, /*1000*/
			isa_xor,
			isa_not_used,
			isa_not_used,
			isa_jmp, /*1100*/
			isa_shf,
			isa_lea,
			isa_trap 	};

void process_instruction(){
  /*  function: process_instruction
   *
   *    Process one instruction at a time
   *       -Fetch one instruction
   *       -Decode
   *       -Execute
   *       -Update NEXT_LATCHES
   */
	int ir = ((MEMORY[CURRENT_LATCHES.PC>>1][1] & 0xFF) << 8) | (MEMORY[CURRENT_LATCHES.PC>>1][0] & 0xFF);
	int opcode = (ir >> 12) & 0xF;
	NEXT_LATCHES.PC = CURRENT_LATCHES.PC + 2;
	isa_ptr[opcode](ir);
}
int sext(int number, int size) {
	int mask = 1 << (size - 1);
	if (number & mask)
	{
		/*If it's negative, return the sign extended value*/
		return -(~number + 1);
	}
	return number;
}
void setCC(int result)
{
	NEXT_LATCHES.N = 0;
	NEXT_LATCHES.Z = 0;
	NEXT_LATCHES.P = 0;
	if (result == 0) {
		NEXT_LATCHES.Z = 1;
	}
	else if (result > 0) {
		NEXT_LATCHES.P = 1;
	}
	else {
		NEXT_LATCHES.N = 1;
	}
}
void isa_add(int word) {
	int dr = (word >> 9) & 0x7;
	int sr1 = (word >> 6) & 0x7;
	int a = (word >> 5) & 0x1;
	if (a == 0)
	{
		int sr2 = word & 0x7;
		NEXT_LATCHES.REGS[dr] = CURRENT_LATCHES.REGS[sr1] + CURRENT_LATCHES.REGS[sr2];
	}
	else
	{
		int imm5 = word & 0x1F;
		NEXT_LATCHES.REGS[dr] = CURRENT_LATCHES.REGS[sr1] + sext(imm5, 5);
	}
	setCC(NEXT_LATCHES.REGS[dr]);
#ifdef DEBUG
	printf("Add instruction 0x%x executed, result 0x%x is at register %d\n", word, NEXT_LATCHES.REGS[dr], dr);
#endif
}
void isa_and(int word) {
}
void isa_br(int word) { /* Check again */
	int n = (word >> 11) & 0x1;
	int z = (word >> 10) & 0x1;
	int p = (word >> 9) & 0x1;
	int offset9 = word & 0x01FF;

	if ((n == 1 && CURRENT_LATCHES.N == 1) || (z == 1 && CURRENT_LATCHES.Z == 1)
		|| (p == 1 && CURRENT_LATCHES.P == 1))
	{
		int lshf = sext(offset9, 9) << 1;
		NEXT_LATCHES.PC = CURRENT_LATCHES.PC + lshf;
	}
#ifdef DEBUG
	printf("br instruction 0x%x executed, branched to address 0x%x\n", word, NEXT_LATCHES.PC);
#endif
}
void isa_jmp(int word) {
}
void isa_jsr(int word) { /* Check again */
	int a = (word >> 11) & 0x01;
	int temp = CURRENT_LATCHES.PC;

	if(a == 1) /* JSR */
	{
		int pcoffset11 = word & 0x7FF;
		int lshf = sext(pcoffset11, 11) >> 1;
		NEXT_LATCHES.PC = CURRENT_LATCHES.PC + lshf;
#ifdef DEBUG
		printf("jsr instruction 0x%x executed, jumped to address 0x%x\n", word, NEXT_LATCHES.PC);
#endif
	}
	else /* JSRR */
	{
		int baser = (word >> 6) & 0x7;
		NEXT_LATCHES.PC = CURRENT_LATCHES.REGS[baser];

#ifdef DEBUG
		printf("jsrr instruction 0x%x executed, jumped to address 0x%x, base register\n", word, NEXT_LATCHES.PC);
#endif
	}
	NEXT_LATCHES.REGS[7] = temp;
}
void isa_ldb(int word) {
	int dr = (word >> 9) & 0x7;
	int baser = (word >> 6) & 0x7;
	int boffset6 = word & 0x3F;
	int mar = baser + sext(boffset6, 6);
	NEXT_LATCHES.REGS[dr] = sext(MEMORY[mar >> 1][mar&1], 8);
	setCC(NEXT_LATCHES.REGS[dr]);
#ifdef DEBUG
	printf("LDB instruction 0x%x executed, result 0x%x is at register %d", word, NEXT_LATCHES.REGS[dr], dr);
#endif
}
void isa_ldw(int word) { /* Check again */
	int dr = (word >> 9) & 0x7;
	int baser = (word >> 6) & 0x7;
	int offset6 = word & 0x3F;
	int lshf = sext(offset6, 6) << 1;
	int mar = CURRENT_LATCHES.REGS[baser] + lshf;
	
	NEXT_LATCHES.REGS[dr] = (MEMORY[mar >> 1][1] << 8) | MEMORY[mar >> 1][0];
	setCC(NEXT_LATCHES.REGS[dr]);
#ifdef DEBUG
	printf("LDW instruction 0x%x executed, result 0x%x is at register %d", word, NEXT_LATCHES.REGS[dr], dr);
#endif
}
void isa_lea(int word) {
	int dr = (word >> 9) & 0x7;
	int pcoffset9 = word & 0x1FF;
	NEXT_LATCHES.REGS[dr] = NEXT_LATCHES.PC + (sext(pcoffset9, 9) << 1);
	setCC(NEXT_LATCHES.REGS[dr]);
}
void isa_rti(int word) {
	/* Do not need to implement RTI */
}
void isa_shf(int word) {
}
void isa_stb(int word) { /* Check again */
	int sr = (word >> 9) & 0x7;
	int baser = (word >> 6) & 0x7;
	int boffset6 = word & 0x3F;
	int mar = CURRENT_LATCHES.REGS[baser] + sext(boffset6, 6);

	MEMORY[mar >> 1][mar & 0x1] = sr & 0xFF;
#ifdef DEBUG
	printf("stb instruction 0x%x executed, result 0x%x is at memory MEM[%d][%d]", word, MEMORY[mar >> 1][mar & 0x1], mar >> 1, mar & 0x1);
#endif
}
void isa_stw(int word) {
}
void isa_trap(int word) { /* Check again */
	int trapvect8 = word & 0xFF;
	int mar = trapvect8 << 1;

	NEXT_LATCHES.REGS[7] = CURRENT_LATCHES.PC;
	NEXT_LATCHES.PC = (MEMORY[mar >> 1][1] << 8) | MEMORY[mar >> 1][0];
#ifdef DEBUG
	printf("trap instruction 0x%x executed, branched to 0x%x", word, MEMORY[mar >> 1][mar & 0x1]);
#endif
}
void isa_xor(int word) {
}
void isa_not_used(int word) {
}

