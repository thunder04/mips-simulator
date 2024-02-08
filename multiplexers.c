// Multi-cycle MIPS Processor Simulator

#include "multiplexers.h"
#include "macros.h"
#include "special_function_registers.h"

unsigned int IorD(enum IorDSel sel) {
  switch (sel) {
  case IorD_C:
    return C;
  case IorD_PC:
    return PC;
  default:
    return 0;
  }
}

unsigned int RegDst(enum RegDstSel sel) {
  switch (sel) {
  case RegDst_Rd:
    return IR_rd();
  case RegDst_Rt:
    return IR_rt();
  default:
    return 0;
  }
}

unsigned int MemToReg(enum MemToRegSel sel) {
  switch (sel) {
  case MemToReg_C:
    return C;
  case MemToReg_DR:
    return DR;
  default:
    return 0;
  }
}

unsigned int ALUSrcA(enum ALUSrcASel sel) {
  switch (sel) {
  case ALUSrcA_A:
    return A;
  case ALUSrcA_PC:
    return PC;
  default:
    return 0;
  }
}

unsigned int ALUSrcB(enum ALUSrcBSel sel) {
  switch (sel) {
  case ALUSrcB_4:
    return 4;
  case ALUSrcB_B:
    return B;
  case ALUSrcB_SignExtend:
    return IMM_SIGN_EXTEND(IR_imm());
  case ALUSrcB_SignExtendSll2:
    return IMM_SIGN_EXTEND(IR_imm()) << 2;
  default:
    return 0;
  }
}

unsigned int ALSUSrcB(enum ALSUSrcBSel sel) {
  switch (sel) {
  case ALSUSrcB_B:
    return B;
  case ALSUSrcB_SignExtend:
    return IMM_SIGN_EXTEND(IR_imm());
  default:
    return 0;
  }
}

unsigned int PCSrc(enum PCSrcSel sel, unsigned int ALUOut) {
  switch (sel) {
  case PCSrc_ALUOut:
    return ALUOut;
  case PCSrc_C:
    return C;
  case PCSrc_IRAddr:
    return (PC & 0xF0000000) | (IR_addr() >> 2);
  default:
    return 0;
  }
}
