#ifndef __VM_H
#define __VM_H

#include "parser_codegen.h"

int base(int l, int base);
void printStack(int sp, int ar[]);
void virtual_machine(instruction *code, int vFlag);
#endif