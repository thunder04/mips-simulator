#ifndef _MEMORY
#define _MEMORY

// 4MB
#define MEM_RESERVED_SPACE 0x00400000
#define MEM_START_OF_PC MEM_RESERVED_SPACE
// 4MB
#define MEM_MAX_PROGRAM_SIZE 0x00400000
// 16MB
#define MEM_MAX_DATA_SPACE 0x001000000
#define MEM_START_OF_SP MEM_MAX_DATA_SPACE

#define MEM_SIZE (MEM_RESERVED_SPACE + MEM_MAX_PROGRAM_SIZE + MEM_MAX_DATA_SPACE)

typedef int MemData;

MemData mem(unsigned int Address, unsigned int MemRead, unsigned int MemWrite,
            unsigned int WriteData);

#endif
