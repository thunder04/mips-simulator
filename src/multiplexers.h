#ifndef _MULTIPLEXERS
#define _MULTIPLEXERS

enum IorDSel {
  IorD_PC,
  IorD_C,
};

enum MemToRegSel {
  MemToReg_A,
  MemToReg_B,
  MemToReg_C,
  MemToReg_D,
  MemToReg_DR,
  MemToReg_PC,
};

enum RegDstSel {
  // Write to register rt
  RegDst_Rt,
  // Write to register rd
  RegDst_Rd,
  // Write to register $ra
  RegDst_Ra,
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
  ALUSrcB_D,
};

// TODO Is this really needed?
enum ALSUSrcASel {
  ALSUSrcA_B,
};

enum ALSUSrcBSel {
  ALSUSrcB_A,
  ALSUSrcB_Shamt,
};

enum PCSrcSel {
  PCSrc_ALUOut,
  PCSrc_A,
  PCSrc_B,
  PCSrc_C,
  PCSrc_IRAddr,
};

unsigned int IorD(enum IorDSel sel);
unsigned int RegDst(enum RegDstSel sel);
unsigned int MemToReg(enum MemToRegSel sel);
unsigned int ALUSrcA(enum ALUSrcASel sel);
unsigned int ALUSrcB(enum ALUSrcBSel sel);
unsigned int ALSUSrcA(enum ALSUSrcASel sel);
unsigned int ALSUSrcB(enum ALSUSrcBSel sel);
unsigned int PCSrc(enum PCSrcSel sel, unsigned int ALUOut);

#endif
