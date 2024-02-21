#pragma once

extern int A, B, C, D, DR, IR, PC;

// Helper functions to obtain each field from IR
unsigned int IR_opcode();
unsigned int IR_rs();
unsigned int IR_rt();
unsigned int IR_rd();
unsigned int IR_shamt();
// unsigned int IR_funct();
unsigned int IR_addr();
unsigned int IR_imm();
