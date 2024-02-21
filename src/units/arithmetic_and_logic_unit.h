#pragma once

#include "../multiplexers.h"

struct ALUOut {
  int Zero;
  unsigned int Out;
};

enum ALUOp {
  ALUOp_Add,
  ALUOp_Sub,
  ALUOp_And,
  ALUOp_Or,
};

struct ALUOut alu(unsigned int a, unsigned int b, enum ALUOp op);
