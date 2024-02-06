#include "macros.h"

unsigned int A = 0, B = 0, C = 0, DR = 0, IR = 0, PC = 0;

unsigned int IR_opcode() { return (IR >> (32 - 6)) & MASK(6); }
unsigned int IR_rs() { return (IR >> (26 - 5)) & MASK(5); }
unsigned int IR_rt() { return (IR >> (21 - 5)) & MASK(5); }
unsigned int IR_rd() { return (IR >> (16 - 5)) & MASK(5); }
unsigned int IR_shamt() { return (IR >> (11 - 5)) & MASK(5); }
unsigned int IR_funct() { return IR & MASK(6); }
unsigned int IR_addr() { return IR & MASK(26); }
unsigned int IR_imm() { return IR & MASK(16); }
