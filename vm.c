#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

/********************* Machine *******************/

void handleInterface(uint8_t iface, uint32_t size) {
  if (iface == 1) {
    printf("%s", iface_buffer);
  }
}

/* Print error message and quit */
void vm_error(char *msg) {
  printf("Error: %s", msg);
  exit(0);
}

void printMemory(uint8_t *pool) {
  int i;
  for (i = 0; i < MEMORY_POOL_SIZE; i++)
    printf("%d: %x\n", i, pool[i]);
}
#endif

/********************** INSTRUCTION ************************/


/* Process opcode */
int processInstr(Stack *stack, Program *prog, uint8_t opcode) {

  if (opcode == ADD_BYTE) {
    uint8_t byte1 = pop(stack);
    uint8_t byte2 = pop(stack);
    push(stack, byte1 + byte2);    
  }

  else if (opcode == SUB_BYTE) {
    uint8_t byte1 = pop(stack);
    uint8_t byte2 = pop(stack);
    push(stack, byte2 - byte1);
  }

  else if (opcode == MUL_BYTE) {
    uint8_t byte1 = pop(stack);
    uint8_t byte2 = pop(stack);
    push(stack, byte1 * byte2);
  }

  else if (opcode == DIV_BYTE) {
    uint8_t byte1 = pop(stack);
    uint8_t byte2 = pop(stack);
    push(stack, byte2 / byte1);
  }

  else if (opcode == MOD_BYTE) {
    uint8_t byte1 = pop(stack);
    uint8_t byte2 = pop(stack);
    push(stack, byte2 % byte1);
  }

  else if (opcode == PUSH_BYTE) {
    uint8_t byteToPush = nextOp(prog);    
    push(stack, byteToPush);
  }

  else if (opcode == STORE_BYTE) {
    /* Address is 32 bit */
    VM_INT32 address = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    memory[address.integer] = pop(stack);
  }

  else if (opcode == LOAD_BYTE) {
    /* Address is 32 bit */
    VM_INT32 address = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    push(stack, memory[address.integer]);
  }

  else if (opcode == HALT) {
    return 0;
  }

  else if (opcode == SEND_INTERFACE) {
    VM_INT32 size = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    VM_INT32 address = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    uint8_t iface = pop(stack);  

    /* copy from VM memory to buffer */
    strncpy(iface_buffer, &memory[address.integer], size.integer);
    handleInterface(iface, size.integer);
  }

  else if (opcode == RECV_INTERFACE) {
    VM_INT32 size = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    VM_INT32 address = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    uint8_t iface = pop(stack); 

    /* copy from interface buffer to VM memory */
    strncpy(&memory[address.integer], iface_buffer, size.integer);
  }

  else if (opcode == AND_BYTE) {
    uint8_t byte1 = pop(stack);
    uint8_t byte2 = pop(stack);
    push(stack, byte1 & byte2);
  }

  else if (opcode == OR_BYTE) {
    uint8_t byte1 = pop(stack);
    uint8_t byte2 = pop(stack);
    push(stack, byte1 | byte2);
  }

  else if (opcode == NOT_BYTE) {
    uint8_t byte = pop(stack);
    push(stack, ~byte);
    }

  else if (opcode == XOR_BYTE) {
    uint8_t byte1 = pop(stack);
    uint8_t byte2 = pop(stack);
    push(stack, byte1 ^ byte2);
    }

  else if (opcode == JUMPG) {
    uint8_t arg2 = pop(stack);
    uint8_t arg1 = pop(stack);
    VM_INT32 address = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    if (arg1 > arg2)
      prog->pointer = address.integer - 1;
  }

  else if (opcode == JUMPE) {
    uint8_t arg2 = pop(stack);
    uint8_t arg1 = pop(stack);
    VM_INT32 address = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    if (arg1 == arg2)
      prog->pointer = address.integer - 1;
  }

  else if (opcode == JUMPL) {
    uint8_t arg2 = pop(stack);
    uint8_t arg1 = pop(stack);
    VM_INT32 address = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    if (arg1 < arg2)
      prog->pointer = address.integer - 1;
  }

  else if (opcode == JUMPNE) {
    uint8_t arg2 = pop(stack);
    uint8_t arg1 = pop(stack);
    VM_INT32 address = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    if (arg1 != arg2)
      prog->pointer = address.integer - 1;
  }

  else if (opcode == JUMPLE) {
    uint8_t arg2 = pop(stack);
    uint8_t arg1 = pop(stack);
    VM_INT32 address = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    if (arg1 <= arg2)
      prog->pointer = address.integer - 1;
  }

  else if (opcode == JUMPGE) {
    uint8_t arg2 = pop(stack);
    uint8_t arg1 = pop(stack);
    VM_INT32 address = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    if (arg1 >= arg2)
      prog->pointer = address.integer - 1;
  }

  else if (opcode == JUMP) {
    VM_INT32 address = {{pop(stack), pop(stack), pop(stack), pop(stack)}};
    prog->pointer = address.integer - 1;
  }

  return 1;
}

/* Get the next opcode from program */
uint8_t nextOp(Program *prog) {
  return prog->buffer[prog->pointer++];
}

void runProgram(Stack *stack, Program *prog) {
  while (processInstr(stack, prog, nextOp(prog)));
    //printStack(stack);
}

int main(void) {
  Stack stack;
  initStack(&stack, MAX_STACK_SIZE);

  // Test program
  uint8_t instructions[] = {PUSH_BYTE, 0x48, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, STORE_BYTE,
                            PUSH_BYTE, 0x45, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x01, STORE_BYTE,
                            PUSH_BYTE, 0x4C, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x02, STORE_BYTE,
                            PUSH_BYTE, 0x4C, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x03, STORE_BYTE,
                            PUSH_BYTE, 0x4F, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x04, STORE_BYTE,
                            PUSH_BYTE, 0x01, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 
                            0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x00, PUSH_BYTE, 0x05, SEND_INTERFACE, HALT};

  Program prog;
  prog.pointer = 0;
  prog.buffer = instructions;

  runProgram(&stack, &prog);
  //printMemory(memory);
  //printStack(&stack);


}


