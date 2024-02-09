#include "register_file.h"
#include "../macros.h"
#include <stdio.h>
#include <stdlib.h>

#define VALIDATE(REG)                                                          \
  if (REG > 31) {                                                              \
    printf(#REG " cannot exceed 31. It was: %u\n", REG);                       \
    exit(1);                                                                   \
  }

// Register File
static unsigned int REG[32] = {0};

struct RegOut reg(unsigned int ReadRegister1, unsigned int ReadRegister2,
                  unsigned int WriteRegister, unsigned int WriteData,
                  unsigned int RegWrite) {
  struct RegOut regOut = {0, 0};

  if (RegWrite) {
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
