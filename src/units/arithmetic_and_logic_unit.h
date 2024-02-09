#ifndef _ARITHMETIC_AND_LOGIC_UNIT
#define _ARITHMETIC_AND_LOGIC_UNIT

#include "../multiplexers.h"

struct ALUOut {
  int Zero;
  unsigned int Out;
};

struct ALUOut alu(unsigned int a, unsigned int b, enum ALUOpSel op);

#endif
