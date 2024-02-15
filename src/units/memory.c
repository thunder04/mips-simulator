#include "memory.h"
#include "../macros.h" // For the DEBUG
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>

#define FROM_IDX (MEM_START_OF_PC)
#define TO_IDX (MEM_START_OF_SP)

static unsigned int *MEM = NULL;

MemData mem(unsigned int Address, unsigned int MemRead, unsigned int MemWrite,
            unsigned int WriteData) {
  // Initialize MEM in the heap, only once because I'm unsure if Windows allows
  // 5MB stack allocation. On Linux, where I am creating this, it's
  // configurable.
  if (MEM == NULL) {
    DEBUG_PRINTF("Allocating %ld bytes of memory\n",
                 sizeof(unsigned int) * MEM_SIZE);

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
    DEBUG_PRINTF("W MEM[%u] <== %u\n", Address, WriteData);
    MEM[Address] = WriteData;
  } else if (MemRead) {
    DEBUG_PRINTF("R MEM[%u] ==> %u\n", Address, MEM[Address]);

    return MEM[Address];
  }

  return 0;
}

void write_mem_stats_section(FILE *fstatsptr) {
  fprintf(fstatsptr, "\n===== Memory =====\n");

  char binaryBuf[sizeof(unsigned int) * 8 + 1] = {0};

  for (int i = FROM_IDX; i < TO_IDX; i += 4) {
    unsigned int val = MEM[i];

    // Most memory addresses contain zeroes, exclude them to reduce clutter
    if (val != 0) {
      format_bits(binaryBuf, val);
      fprintf(fstatsptr, "MEM[%u] = %u = 0b%s\n", i, val, binaryBuf);
    }
  }
}
