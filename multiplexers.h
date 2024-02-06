enum IorD {
  IorD_PC,
  IorD_C,
};

enum MemToReg {
  MemToReg_C,
  MemToReg_DR,
};

enum ALUSrcA {
  ALUSrcA_PC,
  ALUSrcA_A,
};

enum ALUSrcB {
  ALUSrcB_B,
  ALUSrcB_4,
  ALUSrcB_SignExtend,
  ALUSrcB_SignExtendSll2,
};

enum ALUOp {
  ALUOp_Add,
  ALUOp_Sub,
  ALUOp_Funct,
};
