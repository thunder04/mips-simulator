// Multi-cycle MIPS Processor Simulator

#include <stdio.h>
#include <stdlib.h>

// A helper macro to create a mask of n bits. For example: n = 5 => 0b11111
#define MASK(n) ((1 << n) - 1)

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

int main(int argc, char *argv[]) {
  IR = 0b00000001001010100100000000100000; // add $8, $9, $10

  printf("opcode: %d == 0\n", IR_opcode());
  printf("rs: %d == 9\n", IR_rs());
  printf("rt: %d == 10\n", IR_rt());
  printf("rd: %d == 8\n", IR_rd());
  printf("shamt: %d == 0\n", IR_shamt());
  printf("funct: %d == 32\n", IR_funct());
  
  return 0;
}
