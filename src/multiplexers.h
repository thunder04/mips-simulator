#ifndef _MULTIPLEXERS
#define _MULTIPLEXERS

enum IorDSel {
  IorD_PC,
  IorD_C,
};

enum MemToRegSel {
  MemToReg_A,
  MemToReg_C,
  MemToReg_D,
  MemToReg_DR,
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
};

// TODO Is this really needed?
enum ALSUSrcASel {
  ALSUSrcA_A,
};

enum ALSUSrcBSel {
  ALSUSrcB_B,
  ALSUSrcB_Shamt,
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
unsigned int ALSUSrcA(enum ALSUSrcASel sel);
unsigned int ALSUSrcB(enum ALSUSrcBSel sel);
unsigned int PCSrc(enum PCSrcSel sel, unsigned int ALUOut);

#endif
