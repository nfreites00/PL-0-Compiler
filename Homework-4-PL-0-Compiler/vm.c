#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "codegen.h"
#include "vm.h"

#define MAX_DATA_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500

int stack[MAX_DATA_STACK_HEIGHT]; // Globally declaring stack

void virtual_machine(instruction *code, int vFlag) {

  // Initial values for PM/0 CPU registers
  int sp = MAX_DATA_STACK_HEIGHT;
  int bp = sp - 1;
  int pc = 0;
  instruction ir;

  int activationRecord[MAX_CODE_LENGTH];
  int arIndex = 0;

  int index = 0; // Used to keep track of the index of the array of instructions.

  // Flag to signal that the program is finished. If set to zero, the program has ended.
  int haltFlag = 1;
  index = 0;

	if(vFlag == 1) {
		printf("\n-v compiler directive:\n");
		printf("Virtual Machine Execution Trace:\n");
		printf("\t\t\t\t\tpc\t\tbp\t\tsp\t\tstack\n");
  	printf("Initial values\t\t%d\t\t%d\t\t%d\n", pc, bp, sp);
	}

  while(haltFlag == 1) {
    ir = code[pc]; // Fetch cycle.
    pc = pc + 1;
    
    switch(ir.opcode) { // Execute cycle.
      // LIT: pushes constant value (literal) M onto the stack
      case 1:
        sp = sp - 1;
        stack[sp] = ir.m;
				if(vFlag == 1) {
					printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        	printStack(sp, activationRecord);
				}
        index = pc;
        break;
      // OPR: operation to be performed on the data at the top of stack
      case 2:     
        switch(ir.m) {
          case 0: // RET
            sp = bp + 1;
            pc = stack[sp - 4];
            bp = stack[sp - 3];
            if(vFlag == 1){
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
            
          case 1: // NEG
            stack[sp] = stack[sp] * -1;
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
          // ADD
          case 2:
            sp = sp + 1;
            stack[sp] = stack[sp] + stack[sp - 1];
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
          // SUB
          case 3:
            sp = sp + 1;
            stack[sp] = stack[sp] - stack[sp - 1];
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
          // MUL
          case 4:
            sp = sp + 1;
            stack[sp] = stack[sp] * stack[sp - 1];
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
          // DIV
          case 5:
            sp = sp + 1;
            stack[sp] = (stack[sp] / stack[sp - 1]);
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
          // ODD
          case 6:
            stack[sp] = stack[sp] % 2;
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
          // MOD
          case 7:
            sp = sp + 1;
            stack[sp] = stack[sp] % stack[sp - 1];
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
          // EQL
          case 8:
            sp = sp + 1;
            stack[sp] = (stack[sp] == stack[sp - 1]);
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
          // NEQ
          case 9:
            sp = sp + 1;
            stack[sp] = (stack[sp] != stack[sp - 1]);
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
          // LSS
          case 10:
            sp = sp + 1;
            stack[sp] = (stack[sp] < stack[sp - 1]);
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
          // LEQ
          case 11:
            sp = sp + 1;
            stack[sp] = (stack[sp] <= stack[sp - 1]);
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
          // GTR
          case 12:
            sp = sp + 1;
            stack[sp] = (stack[sp] > stack[sp - 1]);
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
          // GEQ
          case 13:
            sp = sp + 1;
            stack[sp] = (stack[sp] >= stack[sp - 1]);
            if(vFlag == 1) {
							printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
        			printStack(sp, activationRecord);
						}
            index = pc;
            break;
        }
        break;

      // LOD: loads value to top of stack from stack location at offset M
      case 3:
        sp = sp - 1;
        stack[sp] = stack[base(ir.l, bp) - ir.m];
        if(vFlag == 1) {
					printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
					printStack(sp, activationRecord);
				}
        index = pc;
        break;

      // STO: stores value to top of stack from stack location at offset M
      case 4:
        stack[base(ir.l, bp) - ir.m] = stack[sp];
        sp = sp + 1;
        if(vFlag == 1) {
					printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
					printStack(sp, activationRecord);
				}
        index = pc;
        break;

      // Call 
      case 5:
        stack[sp - 1] = 0;             // space to return value
        stack[sp - 2] = base(ir.l,bp); // static link
        stack[sp - 3] = bp;            // dynamic link
        stack[sp - 4] = pc;            // return address
        bp = sp - 1;
        pc = ir.m;
        activationRecord[arIndex] = sp;
        arIndex = arIndex + 1;
				if(vFlag == 1) {
					printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
					printStack(sp, activationRecord);
				}
        index = pc;
        break;

      // INC: allocates M locals
      case 6:
        sp = sp - ir.m;
        if(vFlag == 1) {
					printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
					printStack(sp, activationRecord);
				}
        index = pc;
        break;

      // JMP: jumps to instruction M
      case 7:
        pc = ir.m;
        if(vFlag == 1) {
					printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
					printStack(sp, activationRecord);
				}
        index = pc;
        break;

      // JPC: jump to instruction M if top stack element is 0
      case 8:
        if((stack[sp]) == 0)
        {
          pc = ir.m;
        }
        sp = sp + 1;
        if(vFlag == 1) {
					printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
					printStack(sp, activationRecord);
				}
        index = pc;
        break;

      // SIO: writes the top stack element to the screen
      case 9:
        printf("Write: %d\n", stack[sp]);
        sp = sp + 1;
        if(vFlag == 1) {
					printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
					printStack(sp, activationRecord);
				}
        index = pc;
        break;

      case 10: // SIO: read in input from the user and stores input on top of the stack
        sp = sp - 1;
				printf("Read: ");
        scanf("%d", &stack[sp]);
        if(vFlag == 1) {
					printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
					printStack(sp, activationRecord);
				}
        index = pc;
        break;

      case 11: // SIO: end of program
        haltFlag = 0; // set Halt Flag to zero
        if(vFlag == 1) {
					printf("%d\t%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t", index, ir.op, ir.l, ir.m, pc, bp, sp);
					printStack(sp, activationRecord);
				}
        break;
    }
  }
  return;
}

int base(int l, int base) { // This function finds a variable in a different activation record some L levels down (l stands for L in the instruction format)
  int b1; // Find base L levels down
  b1 = base;
  while(l > 0) {
    b1 = stack[b1 - 1];
    l--;
  }
  return b1;
}

void printStack(int sp, int ar[]) {
  int i = 0;
  for(int j = MAX_DATA_STACK_HEIGHT - 1; j >= sp; j--) {
    printf("%d ", stack[j]);

    if (ar[i] == j) {
      printf("| ");
      i++;
    }
  }
  printf("\n");
}