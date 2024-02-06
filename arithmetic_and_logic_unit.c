#include "arithmetic_and_logic_unit.h"

struct ALUOut alu(unsigned int a, unsigned int b, enum ALUOp op) {
  struct ALUOut out = {.Zero = 1, .Out = 0};

  switch (op) {
  case ALUOp_ADD:
    out.Out = a + b;
    break;
  case ALUOp_SUB:
    out.Out = a - b;
    break;
  case ALUOp_OR:
    out.Out = a | b;
    break;
  case ALUOp_AND:
    out.Out = a & b;
    break;
  }

  if (out.Out == 0)
    out.Zero = 1;

  return out;
}
