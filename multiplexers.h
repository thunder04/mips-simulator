enum IorDSel {
  IorD_PC,
  IorD_C,
};

enum MemToRegSel {
  MemToReg_C,
  MemToReg_DR,
};

enum RegDstSel {
  RegDst_Rt,
  RegDst_Rd,
};

enum ALUSrcASel {
  ALUSrcA_PC,
  ALUSrcA_A,
};

enum ALUSrcBSel {
  ALUSrcB_B,
  ALUSrcB_4,
  ALUSrcB_SignExtend,
  ALUSrcB_SignExtendSll2,
};

// TODO Is this really needed?
enum ALSUSrcASel {
  ALSUSrcA_A,
};

enum ALSUSrcBSel {
  ALSUSrcB_B,
  ALSUSrcB_SignExtend,
};

enum ALUOpSel {
  ALUOp_Add,
  ALUOp_Sub,
  ALUOp_Funct,
};

enum PCSrcSel {
  PCSrc_ALUOut,
  PCSrc_C,
  PCSrc_IRAddr,
};

unsigned int IorD(enum IorDSel sel);
unsigned int RegDst(enum RegDstSel sel);
unsigned int MemToReg(enum MemToRegSel sel);
unsigned int ALUSrcA(enum ALUSrcASel sel);
unsigned int ALUSrcB(enum ALUSrcBSel sel);
unsigned int PCSrc(enum PCSrcSel sel, unsigned int ALUOut);
