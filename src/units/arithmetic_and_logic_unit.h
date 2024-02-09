#include "../multiplexers.h"

struct ALUOut {
  int Zero;
  unsigned int Out;
};

struct ALUOut alu(unsigned int a, unsigned int b, enum ALUOpSel op);
