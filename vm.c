#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "vm.h"


/********************* STACK *******************/

/* Pushes a byte onto the stack */
void push(Stack *stack, uint8_t byte) {
  stack->buffer[stack->pointer++] = byte;
}

/* Pops a byte from the stack */
uint8_t pop(Stack *stack) {

  /* Dont't allow an empty stack to be popped in debug mode */  
  #ifdef DEBUG
  if (stack->pointer == 0)
    vm_error("Attempted to pop empty stack");
  #endif

  return stack->buffer[--stack->pointer];
}

/* Initializes a stack and reserves buffer space */
void initStack(Stack *stack, size_t size) {
  stack->pointer = 0;
  stack->size = 0;
  stack->buffer = (uint8_t*)malloc(size);
}

#ifdef DEBUG
/* Prints the contents of a stack (Right side being top of stack) */
void printStack(Stack *stack) {
  printf("[ ");

  int size = stack->pointer;
  int i;
  for(i = 0; i < size; i++)
    printf("0x%.2x ", stack->buffer[i]);
  
  printf("]<-- \n");
}

/* Print error message and quit */
void vm_error(char *msg) {
  printf("Error: %s", msg);
  exit(0);
}
#endif

/********************** INSTRUCTION ************************/


/* Process opcode */
int processInstr(Stack *stack, Program *prog, uint8_t opcode) {

  if (opcode == ADD_INT32) {
    
    /* Collect the bytes and add as integer */
    VM_INT32 intUnion1 = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    VM_INT32 intUnion2 = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    VM_INT32 sumUnion = {.integer = intUnion1.integer + intUnion2.integer};

    /* Decompose sum bytes and push back onto stack (Pushed MSB first) */  
    push(stack, sumUnion.bytes.byte3);
    push(stack, sumUnion.bytes.byte2);
    push(stack, sumUnion.bytes.byte1);
    push(stack, sumUnion.bytes.byte0); 
  }

  else if (opcode == PUSH) {
    uint8_t byteToPush = nextOp(prog);    
    push(stack, byteToPush);
  }

  else if (opcode == HALT) {
    return 0;
  }

  return 1;
}

/* Get the next opcode from program */
uint8_t nextOp(Program *prog) {
  return prog->buffer[prog->pointer++];
}

void runProgram(Stack *stack, Program *prog) {
  while (processInstr(stack, prog, nextOp(prog)))
    printStack(stack);
}

int main(void) {
  Stack stack;
  initStack(&stack, MAX_STACK_SIZE);

  // Test program
  uint8_t instructions[] = {PUSH, 0x00, PUSH, 0x00, PUSH, 0x03, PUSH, 0xE9, PUSH, 0x00, PUSH, 0x00, PUSH, 0x02, PUSH, 0x39, ADD_INT32, HALT};
  Program prog;
  prog.pointer = 0;
  prog.buffer = instructions;

  runProgram(&stack, &prog);
  printStack(&stack);


}


