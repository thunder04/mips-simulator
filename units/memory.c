#include "memory.h"
#include "../macros.h"
#include <stdio.h>
#include <stdlib.h>

static unsigned int *MEM = NULL;

MemData mem(unsigned int Address, unsigned int MemRead, unsigned int MemWrite,
            unsigned int WriteData) {
  // Initialize MEM in the heap, only once because I'm unsure if Windows allows
  // 5MB stack allocation. On Linux, where I am creating this, it's
  // configurable.
  if (MEM == NULL) {
#ifdef DEBUG
    printf("[DEBUG] Allocating %ld bytes of memory\n",
           sizeof(unsigned int) * MEM_SIZE);
#endif

    MEM = (unsigned int *)malloc(sizeof(unsigned int) * MEM_SIZE);

    if (MEM == NULL) {
      perror("Allocation failure");
      exit(1);
    }
  }

  if (Address > MEM_SIZE) {
    printf("Address exceeds memory.\n");
    exit(1);
  }

  if (MemWrite) {
#ifdef DEBUG
    printf("[DEBUG] W MEM[%u] <== %u\n", Address, WriteData);
#endif

    MEM[Address] = WriteData;
  } else if (MemRead) {
#ifdef DEBUG
    printf("[DEBUG] R MEM[%u] ==> %u\n", Address, MEM[Address]);
#endif

    return MEM[Address];
  }

  return 0;
}
