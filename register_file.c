#include "register_file.h"

// Register File
static int REG[32] = {0};

struct RegOut reg(int ReadRegister1, int ReadRegister2, int WriteRegister,
                  int WriteData, int RegWrite) {
  struct RegOut regOut = {0, 0};

  if (RegWrite) {
    REG[WriteRegister] = WriteData;
  } else {
    regOut.ReadData1 = REG[ReadRegister1];
    regOut.ReadData2 = REG[ReadRegister2];
  }

  return regOut;
}
