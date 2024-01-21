// Assignment: Homework #1 (P-Machine)
// Students: Nicole Freites & Shivani Kumar

#include <stdio.h>
#include <stdlib.h>

#define MAX_DATA_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500

typedef struct instruction{
	int op; // opcode
	int l;  // L
	int m;  // M
}instruction;

int stack[MAX_DATA_STACK_HEIGHT]; // globally declaring stack

// This function finds a variable in a different activation record some L levels down
int base(int l, int base) // l stands for L in the instruction format
{
  int b1; // Find base L levels down
  b1 = base;
  while(l > 0)
  {
    b1 = stack[b1 - 1];
    l--;
  }
  return b1;
}

// Function to print the stack.
void printStack(int sp, int ar[])
{
  int i = 0;
  for(int j = MAX_DATA_STACK_HEIGHT - 1; j >= sp; j--)
  {
    printf("%d ", stack[j]);

    if (ar[i] == j)
    {
      printf("| ");
      i++;
    }
  }
  printf("\n");
}

int main()
{
  // Initialization of the array that will store the instructions from the input file.
  instruction *code = calloc(MAX_CODE_LENGTH, sizeof(instruction));

  // Initial values for PM/0 CPU registers
  int sp = MAX_DATA_STACK_HEIGHT;
  int bp = sp - 1;
  int pc = 0;
  instruction ir;

  int activationRecord[MAX_CODE_LENGTH];
  int arIndex = 0;

 // Used to keep track of the index of the array of instructions.
  int index = 0;

  printf("Line\tOP\t\tL\t\tM\n");

  // Load data while the end of the file is not reached.
  while(!feof(stdin))
  {
    scanf("%d %d %d", &code[index].op, &code[index].l, &code[index].m);

    switch(code[index].op)
    {
      case 1:
        printf(" %d\t\tlit\t\t0\t\t%d\n", index, code[index].m);
        break;

      case 2:
        printf(" %d\t\topr\t\t0\t\t%d\n", index, code[index].m);
        break;

      case 3:
        printf(" %d\t\tlod\t\t%d\t\t%d\n", index, code[index].l, code[index].m);
        break;

      case 4:
        printf(" %d\t\tsto\t\t%d\t\t%d\n", index, code[index].l, code[index].m);
        break;

      case 5:
        printf(" %d\t\tcal\t\t%d\t\t%d\n", index, code[index].l, code[index].m);
        break;

      case 6:
        printf(" %d\t\tinc\t\t0\t\t%d\n", index, code[index].m);
        break;

      case 7:
        printf(" %d\t\tjmp\t\t0\t\t%d\n", index, code[index].m);
        break;

      case 8:
        printf(" %d\t\tjpc\t\t0\t\t%d\n", index, code[index].m);
        break;

      case 9:
        printf(" %d\t\tsio\t\t0\t\t1\n", index);
        break;

      case 10:
        printf(" %d\t\tsio\t\t0\t\t2\n", index);
        break;

      case 11:
        printf(" %d\t\tsio\t\t0\t\t3\n", index);
        break;
    }

    index++;
  }

  // Flag to signal that the program is finished. If set to zero, the program has ended.
  int haltFlag = 1;
  index = 0;

  printf("\n\n\t\t\t\t\tpc\t\tbp\t\tsp\t\tstack\n");
  printf("Initial values\t\t%d\t\t%d\t\t%d\n", pc, bp, sp);

  while(haltFlag == 1)
  {
    // Fetch cycle.
    ir = code[pc];
    pc = pc + 1;
    
    // Execute cycle.
    switch(ir.op)
    {
      // LIT: pushes constant value (literal) M onto the stack
      case 1:
        sp = sp - 1;
        stack[sp] = ir.m;
        printf("%d\tlit\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);
        
        index = pc;
        break;

      // OPR: operation to be performed on the data at the top of stack
      case 2:     
        switch(ir.m)
        {
          // RET
          case 0:
            sp = bp + 1;
            pc = stack[sp - 4];
            bp = stack[sp - 3];
             printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // NEG
          case 1:
            stack[sp] = stack[sp] * -1;
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // ADD
          case 2:
            sp = sp + 1;
            stack[sp] = stack[sp] + stack[sp - 1];
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // SUB
          case 3:
            sp = sp + 1;
            stack[sp] = stack[sp] - stack[sp - 1];
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // MUL
          case 4:
            sp = sp + 1;
            stack[sp] = stack[sp] * stack[sp - 1];
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // DIV
          case 5:
            sp = sp + 1;
            stack[sp] = (stack[sp] / stack[sp - 1]);
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // ODD
          case 6:
            stack[sp] = stack[sp] % 2;
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // MOD
          case 7:
            sp = sp + 1;
            stack[sp] = stack[sp] % stack[sp - 1];
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // EQL
          case 8:
            sp = sp + 1;
            stack[sp] = (stack[sp] == stack[sp - 1]);
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // NEQ
          case 9:
            sp = sp + 1;
            stack[sp] = (stack[sp] != stack[sp - 1]);
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // LSS
          case 10:
            sp = sp + 1;
            stack[sp] = (stack[sp] < stack[sp - 1]);
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // LEQ
          case 11:
            sp = sp + 1;
            stack[sp] = (stack[sp] <= stack[sp - 1]);
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // GTR
          case 12:
            sp = sp + 1;
            stack[sp] = (stack[sp] > stack[sp - 1]);
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;

          // GEQ
          case 13:
            sp = sp + 1;
            stack[sp] = (stack[sp] >= stack[sp - 1]);
            printf("%d\topr\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
            printStack(sp, activationRecord);
            index = pc;
            break;
        }
        break;

      // LOD: loads value to top of stack from stack location at offset M
      case 3:
        sp = sp - 1;
        stack[sp] = stack[base(code[sp].l, bp) - code[sp].m];
         printf("%d\tlod\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);
        index = pc;
        break;

      // STO: stores value to top of stack from stack location at offset M
      case 4:
        stack[base(ir.l, bp) - ir.m] = stack[sp];
        sp = sp + 1;

        printf("%d\tsto\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);

        printStack(sp, activationRecord);
        index = pc;
        break;

      // CAL: calls procedure at code index M
      case 5:
        stack[sp - 1] = 0;             // space to return value
        stack[sp - 2] = base(ir.l,bp); // static link
        stack[sp - 3] = bp;            // dynamic link
        stack[sp - 4] = pc;            // return address
        bp = sp - 1;
        pc = ir.m;
        printf("%d\tcal\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
        activationRecord[arIndex] = sp;
        arIndex = arIndex + 1;
        printStack(sp, activationRecord);
        index = pc;
        break;

      // INC: allocates M locals
      case 6:
        sp = sp - ir.m;
         printf("%d\tinc\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);
        index = pc;
        break;

      // JMP: jumps to instruction M
      case 7:
        pc = ir.m;
         printf("%d\tjmp\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);
        index = pc;
        break;

      // JPC: jump to instruction M if top stack element is 0
      case 8:
        if((stack[sp])==0)
        {
          pc = ir.m;
        }
         printf("%d\tjpc\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);
        sp = sp + 1;
        index = pc;
        break;

      // SIO: writes the top stack element to the screen
      case 9:
        printf("%d", stack[sp]);
        sp = sp + 1;
         printf("%d\tsio\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);
        index = pc;
        break;

      // SIO: read in input from the user and stores input on top of the stack
      case 10:
        sp = sp - 1;
        scanf("%d", &stack[sp]);
        printf("%d\tsio\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.l, ir.m, pc, bp, sp);
        printStack(sp, activationRecord);
        index = pc;
        break;

      // SIO: end of program
      case 11:
        haltFlag = 0; // set Halt Flag to zero
        printf("%d\tsio\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t\n", index, ir.l, ir.m, pc, bp, sp);
        break;
    }
  }
  
  free(code);
  code = NULL;

  return 0;  
}