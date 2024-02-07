#include "memory.h"
#include "stdio.h"
#include "stdlib.h"

static unsigned int *MEM = NULL;

MemData mem(unsigned int Address, unsigned int MemRead, unsigned int MemWrite,
            unsigned int WriteData) {
  // Initialize MEM in the heap, only once because I'm unsure if Windows allows
  // 5MB stack allocation. On Linux, where I am creating this, it's
  // configurable.
  if (MEM == NULL) {
    MEM = (unsigned int *)malloc(sizeof(unsigned int) * MEM_SIZE);

    if (MEM == NULL) {
      perror("Allocation failure");
      exit(1);
    }
  }

  if (MemWrite) {
    MEM[Address] = WriteData;
  } else if (MemRead) {
    return MEM[Address];
  }

  return 0;
}
