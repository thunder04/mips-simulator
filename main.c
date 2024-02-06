// Multi-cycle MIPS Processor Simulator

#include "arithetic_and_logic_unit.h"
#include "macros.h"
#include "memory.h"
#include "multiplexers.h"
#include "register_file.h"
#include "special_function_registers.h"
#include <stdio.h>
#include <stdlib.h>

// Instruction Counter and μPC
static unsigned int IC = 0, microPC = 0;
// CPU Clock
static unsigned int clock = 0;

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
