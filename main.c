// Multi-cycle MIPS Processor Simulator

#include "arithetic_and_logic_unit.h"
#include "memory.h"
#include "multiplexers.h"
#include "register_file.h"
#include <stdio.h>
#include <stdlib.h>

// A helper macro to create a mask of n bits. For example: n = 5 => 0b11111
#define MASK(n) ((1 << (n)) - 1)
// A helper macro that implements the sign extend function for the immediate
// field (of 16 bits) of IR.
// Explanation: If the 16th bit (15 if counting from 0) (the sign) is 1, then
// fill the rest of the integer with 1s by ORing a mask of 16 bits, moved to the
// MSB with imm itself. Otherwise, leave it as it is.
#define IMM_SIGN_EXTEND(imm)                                                   \
  (((imm) >> 15) == 1 ? (MASK(16) << 16) | (imm) : (imm))

// Special function registers
static unsigned int A = 0, B = 0, C = 0, DR = 0, IR = 0, PC = 0, microPC = 0;
// CPU Clock
static unsigned int clock = 0;
// Instruction Counter
static unsigned int IC = 0;

// Helper functions to obtain each field from IR
unsigned int IR_opcode() { return (IR >> (32 - 6)) & MASK(6); }
unsigned int IR_rs() { return (IR >> (26 - 5)) & MASK(5); }
unsigned int IR_rt() { return (IR >> (21 - 5)) & MASK(5); }
unsigned int IR_rd() { return (IR >> (16 - 5)) & MASK(5); }
unsigned int IR_shamt() { return (IR >> (11 - 5)) & MASK(5); }
unsigned int IR_funct() { return IR & MASK(6); }
unsigned int IR_addr() { return IR & MASK(26); }
unsigned int IR_imm() { return IR & MASK(16); }

// Helper functions to select source according to selection signal
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

int main(int argc, char *argv[]) {
  IR = 0b00000001001010100100000000100000; // add $8, $9, $10

  printf("opcode: %d == 0\n", IR_opcode());
  printf("rs: %d == 9\n", IR_rs());
  printf("rt: %d == 10\n", IR_rt());
  printf("rd: %d == 8\n", IR_rd());
  printf("shamt: %d == 0\n", IR_shamt());
  printf("funct: %d == 32\n", IR_funct());

  unsigned int imm1 = MASK(15) + 1;
  printf("imm1: %d == %d", imm1, IMM_SIGN_EXTEND(imm1));

  return 0;
}
