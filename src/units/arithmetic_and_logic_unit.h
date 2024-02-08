struct ALUOut {
  int Zero;
  unsigned int Out;
};

// Unlike the real implementation, this enum doesn't represent the ALUOp
// multiplexer, but the ALU's signal.
enum ALUOp { ALUOp_ADD, ALUOp_SUB, ALUOp_OR, ALUOp_AND };

struct ALUOut alu(unsigned int a, unsigned int b, enum ALUOp op);
