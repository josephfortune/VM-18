/* #define LITTLE_ENDIAN in case its not included */
#define MAX_STACK_SIZE 100
#define DEBUG

/* opcodes */
#define ADD_INT32 0
#define PUSH 1
#define HALT 2

/* Stack */
typedef struct {
  int pointer;
  int size;
  uint8_t *buffer;
} Stack;

/* Program */
typedef struct {
  int pointer;
  uint8_t *buffer;
} Program;

/****************** Data Types ******************/

#ifdef BIG_ENDIAN

/* Union for accessing individual bytes of 32 bit integer */
typedef union {
  struct {  
    uint8_t byte0, byte1, byte2, byte3; /* 0 LSB --- 3 MSB */
  } bytes;
  uint32_t integer;
} VM_INT32;

#else
/* Union for accessing individual bytes of 32 bit integer */
typedef union {
  struct {
    uint8_t byte3, byte2, byte1, byte0; /* 0 LSB --- 3 MSB */
  } bytes;
  uint32_t integer;
} VM_INT32;
#endif


/****************** Functions ******************/

/* Stack */
void push(Stack *stack, uint8_t byte);
uint8_t pop(Stack *stack);
void initStack(Stack *stack, size_t size);

/* Program */
uint8_t nextOp(Program *prog);
int processInstr(Stack *stack, Program *prog, uint8_t opcode);
void runProgram(Stack *stack, Program *prog);


#ifdef DEBUG
void printStack(Stack *stack);
void vm_error(char *msg);
#endif

