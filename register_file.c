#include "register_file.h"

// Register File
static unsigned int REG[32] = {0};

struct RegOut reg(unsigned int ReadRegister1, unsigned int ReadRegister2,
                  unsigned int WriteRegister, unsigned int WriteData,
                  unsigned int RegWrite) {
  struct RegOut regOut = {0, 0};

  if (RegWrite) {
    REG[WriteRegister] = WriteData;
  } else {
    regOut.ReadData1 = REG[ReadRegister1];
    regOut.ReadData2 = REG[ReadRegister2];
  }

  return regOut;
}
