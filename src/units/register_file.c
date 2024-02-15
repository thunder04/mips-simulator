#include "register_file.h"
#include "../macros.h"
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>

#define VALIDATE(REG)                                                          \
  if (REG > 31) {                                                              \
    printf(#REG " cannot exceed 31. It was: %u\n", REG);                       \
    exit(EXIT_FAILURE);                                                        \
  }

// Register File
static unsigned int REG[32] = {0};

struct RegOut reg(unsigned int ReadRegister1, unsigned int ReadRegister2,
                  unsigned int WriteRegister, unsigned int WriteData,
                  unsigned int RegWrite) {
  struct RegOut regOut = {0, 0};

  if (RegWrite) {
    if (WriteRegister == 0 && WriteData != 0) {
      printf("Register $0 is readonly.\n");
      exit(EXIT_FAILURE);
    }

    VALIDATE(WriteRegister);
    DEBUG_PRINTF("W REG[%u] <== %u\n", WriteRegister, WriteData);

    REG[WriteRegister] = WriteData;
  } else {
    VALIDATE(ReadRegister1);
    VALIDATE(ReadRegister2);
    DEBUG_PRINTF("R RD1: REG[%u] ==> %u\tRD2: REG[%u] ==> %u\n", ReadRegister1,
                 REG[ReadRegister1], ReadRegister2, REG[ReadRegister2]);

    regOut.ReadData1 = REG[ReadRegister1];
    regOut.ReadData2 = REG[ReadRegister2];
  }

  return regOut;
}

void write_reg_stats_section(FILE *fstatsptr) {
  fprintf(fstatsptr, "\n===== Registers =====\n");

  char binaryBuf[sizeof(unsigned int) * 8 + 1] = {0};

  for (int i = 0; i < 32; ++i) {
    unsigned int val = REG[i];

    format_bits(binaryBuf, val);
    fprintf(fstatsptr, "$%d = %u = 0b%s\n", i, val, binaryBuf);
  }
}
