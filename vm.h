/* #define LITTLE_ENDIAN in case its not included */
#define MAX_STACK_SIZE 100
#define MEMORY_POOL_SIZE 256
#define IFACE_BUFFER_SIZE 256
#define DEBUG

/* opcodes */
#define PUSH_BYTE 1
#define HALT 2
#define ADD_BYTE 3
#define SUB_BYTE 4
#define MUL_BYTE 5
#define DIV_BYTE 6
#define MOD_BYTE 7
#define LOAD_BYTE 8
#define STORE_BYTE 9
#define SEND_INTERFACE 10
#define RECV_INTERFACE 11
#define AND_BYTE 12
#define OR_BYTE 13
#define NOT_BYTE 14
#define XOR_BYTE 15
#define JUMPG 16
#define JUMPE 17
#define JUMPL 18
#define JUMPNE 19
#define JUMPLE 20
#define JUMPGE 21
#define JUMP 22

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

/* Memory Pool */
uint8_t memory[MEMORY_POOL_SIZE];

/* Interface buffer */
uint8_t iface_buffer[IFACE_BUFFER_SIZE];

/****************** Data Types ******************/

/* Note that these 32 bit structures are not part of the virtual machines datatypes.
 * they are merely this VM's way of converting 4 bytes describing a memory address.
 * If this were written in java, it would probably be handled through bitwise operations
 * between the individual bytes and a java native datatype. */


/* VM_INT32 Unions are to be used by pushing 4 bytes on to the stack MSB first!!! */
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

/* Machine */
void handleInterface(uint8_t iface, uint32_t size);

#ifdef DEBUG
void printStack(Stack *stack);
void vm_error(char *msg);
#endif

