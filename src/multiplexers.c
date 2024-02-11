#include "multiplexers.h"
#include "macros.h"
#include "special_function_registers.h"

unsigned int IorD(enum IorDSel sel) {
  switch (sel) {
  case IorD_A:
    return A;
  case IorD_B:
    return B;
  case IorD_C:
    return C;
  case IorD_PC:
    return PC;
  }
}

unsigned int RegDst(enum RegDstSel sel) {
  switch (sel) {
  case RegDst_Rd:
    return IR_rd();
  case RegDst_Rt:
    return IR_rt();
  case RegDst_Ra:
    return 31;
  }
}

unsigned int MemToReg(enum MemToRegSel sel) {
  switch (sel) {
  case MemToReg_A:
    return A;
  case MemToReg_B:
    return B;
  case MemToReg_C:
    return C;
  case MemToReg_D:
    return D;
  case MemToReg_DR:
    return DR;
  case MemToReg_PC:
    return PC;
  }
}

unsigned int ALUSrcA(enum ALUSrcASel sel) {
  switch (sel) {
  case ALUSrcA_A:
    return A;
  case ALUSrcA_B:
    return B;
  case ALUSrcA_PC:
    return PC;
  }
}

unsigned int ALUSrcB(enum ALUSrcBSel sel) {
  switch (sel) {
  case ALUSrcB_4:
    return 4;
  case ALUSrcB_B:
    return B;
  case ALUSrcB_D:
    return D;
  case ALUSrcB_DR:
    return DR;
  case ALUSrcB_SignExtend:
    return IMM_SIGN_EXTEND(IR_imm());
  case ALUSrcB_SignExtendSll2:
    return IMM_SIGN_EXTEND(IR_imm()) << 2;
  }
}

unsigned int ALSUSrcB(enum ALSUSrcBSel sel) {
  switch (sel) {
  case ALSUSrcB_A:
    return A;
  case ALSUSrcB_Shamt:
    return IR_shamt();
  }
}

unsigned int PCSrc(enum PCSrcSel sel, unsigned int ALUOut) {
  switch (sel) {
  case PCSrc_ALUOut:
    return ALUOut;
  case PCSrc_A:
    return A;
  case PCSrc_C:
    return C;
  case PCSrc_IRAddr:
    return (PC - 4) + ((PC & 0xF0000000) | (IR_addr() << 2));
  }
}
