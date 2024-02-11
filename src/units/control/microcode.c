#include "microcode.h"
#include <stdio.h>
#include <stdlib.h>

#define JUMP_TABLE_SIZE 63

// Be mindful when you change the order of the microcode. Only append to the
// end. If you end up needing an extra cycle for an already implemented
// instruction, do NOT forget to update the jump table and the comments.
// clang-format off
const struct Microinstruction MICROCODE[] = {
  // 0 - fetch: PC += 4; C = PC + imm
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_PC, .alu2 = ALUSrcB_4, .mem = mMCread_pc, .pc = mPWCalu_out },
  { .sequencing = { mSKdisp }, .alu = mACadd, .alu1 = ALUSrcA_PC, .alu2 = ALUSrcB_SignExtendSll2, .rf = mRCread_rs_rt },
  // 2 - break:
  { .sequencing = { mSKlabel, 0 }, .exit = mECexit },
  // 3 - add: rd = rs + rt
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 5 - sub: rd = rs - rt
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 7 - or: rd = rs | rt
  { .sequencing = { mSKseq }, .alu = mACor, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 9 - and: rd = rs & rt
  { .sequencing = { mSKseq }, .alu = mACand, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 11 - sllv: rd = rt << rs
  { .sequencing = { mSKseq }, .alsu = mASCsll, .alsu2 = ALSUSrcB_A },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 13 - srlv: rd = rt >> rs
  { .sequencing = { mSKseq }, .alsu = mASCsrl, .alsu2 = ALSUSrcB_A },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 15 - srav: rd = rt >>> rs
  { .sequencing = { mSKseq }, .alsu = mASCsra, .alsu2 = ALSUSrcB_A },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 17 - rorv: rd = rt ROR rs
  { .sequencing = { mSKseq }, .alsu = mASCror, .alsu2 = ALSUSrcB_A },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 19 - addi: rt = rs + imm
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rt_c },
  // 21 - ori: rt = rs | imm
  { .sequencing = { mSKseq }, .alu = mACor, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rt_c },
  // 23 - andi: rt = rs & imm
  { .sequencing = { mSKseq }, .alu = mACand, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rt_c },
  // 25 - sll: rd = rt << shamt
  { .sequencing = { mSKseq }, .alsu = mASCsll, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 27 - srl: rd = rt >> shamt
  { .sequencing = { mSKseq }, .alsu = mASCsrl, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 29 - sra: rd = rt >>> shamt
  { .sequencing = { mSKseq }, .alsu = mASCsra, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 31 - ror: rd = rt ROR shamt
  { .sequencing = { mSKseq }, .alsu = mASCror, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 33 - beq: if (rs == rt) PC = C
  { .sequencing = { mSKlabel, 0 }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B, .pc = mPWCc_cond },
  // 34 - bne: if (rs != rt) PC = C
  { .sequencing = { mSKlabel, 0 }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B, .pc = mPWCc_not_cond },
  // 35 - j: PC = addr
  { .sequencing = { mSKlabel, 0 }, .pc = mPWCjump_address },
  // 36 - jr: PC = rs
  { .sequencing = { mSKlabel, 0 }, .pc = mPWCa },
  // 37 - jal: $31 = PC; PC = addr
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_ra_pc, .pc = mPWCjump_address },
  // 38 - jalr: $31 = rd; PC = rs
  { .sequencing = { mSKseq }, .rf = mRCread_rs_rd },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_ra_b, .pc = mPWCa },
  // 40 - slladd: D = rt << shamt; rd = rs + D
  { .sequencing = { mSKseq }, .alsu = mASCsll, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 43 - srladd: D = rt >> shamt; rd = rs + D
  { .sequencing = { mSKseq }, .alsu = mASCsrl, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 46 - sraadd: D = rt >>> shamt; rd = rs + D
  { .sequencing = { mSKseq }, .alsu = mASCsra, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 49 - roradd: D = rt ROR shamt; rd = rs + D
  { .sequencing = { mSKseq }, .alsu = mASCror, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 52 - sllsub: D = rt << shamt; rd = rs - D
  { .sequencing = { mSKseq }, .alsu = mASCsll, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 55 - srlsub: D = rt >> shamt; rd = rs - D
  { .sequencing = { mSKseq }, .alsu = mASCsrl, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 58 - srasub: D = rt >>> shamt; rd = rs - D
  { .sequencing = { mSKseq }, .alsu = mASCsra, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 61 - rorsub: D = rt ROR shamt; rd = rs - D
  { .sequencing = { mSKseq }, .alsu = mASCror, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 64 - lw: rt = MEM[rs + imm]
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKseq }, .mem = mMCread_c },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rt_dr },
  // 67 - sw: MEM[rs + imm] = rt
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .mem = mMCwrite_c },

  ////////////////////////////////////////////////////////////////////////////////////////
  // NOTE: I improvised for the following instructions since they do not exist anywhere //
  ////////////////////////////////////////////////////////////////////////////////////////
  
  // 69 - addm: DR = MEM[rs + imm]; rt = rt + DR
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKseq }, .mem = mMCread_c },
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_B, .alu2 = ALUSrcB_DR },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rt_c },
  // 73 - addm: A = MEM[rs]; B = MEM[rt]; rd = A + B
  { .sequencing = { mSKseq }, .mem = mMCread_a },
  { .sequencing = { mSKseq }, .mem = mMCread_b },
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 77 - subm: DR = MEM[rs + imm]; rt = rt - DR
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKseq }, .mem = mMCread_c },
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_B, .alu2 = ALUSrcB_DR },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rt_c },
  // 81 - subm: A = MEM[rs]; B = MEM[rt]; rd = A - B
  { .sequencing = { mSKseq }, .mem = mMCread_a },
  { .sequencing = { mSKseq }, .mem = mMCread_b },
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
};
// clang-format on

// Do NOT change the order of the jump table. It relies on index position
// Auto-generated by gen_jump_table.py
static int OPCODE_JUMP_TABLE[JUMP_TABLE_SIZE] = {
    3,  // INSTRUCTION: 0b000000 => add
    5,  // INSTRUCTION: 0b000001 => sub
    7,  // INSTRUCTION: 0b000010 => or
    9,  // INSTRUCTION: 0b000011 => and
    11, // INSTRUCTION: 0b000100 => sllv
    13, // INSTRUCTION: 0b000101 => srlv
    15, // INSTRUCTION: 0b000110 => srav
    17, // INSTRUCTION: 0b000111 => rorv
    19, // INSTRUCTION: 0b001000 => addi
    -1, // NOTHING
    21, // INSTRUCTION: 0b001010 => ori
    23, // INSTRUCTION: 0b001011 => andi
    25, // INSTRUCTION: 0b001100 => sll
    27, // INSTRUCTION: 0b001101 => srl
    29, // INSTRUCTION: 0b001110 => sra
    31, // INSTRUCTION: 0b001111 => ror
    33, // INSTRUCTION: 0b010000 => beq
    34, // INSTRUCTION: 0b010001 => bne
    -1, // NOTHING
    2,  // INSTRUCTION: 0b010011 => break
    35, // INSTRUCTION: 0b010100 => j
    36, // INSTRUCTION: 0b010101 => jr
    37, // INSTRUCTION: 0b010110 => jal
    38, // INSTRUCTION: 0b010111 => jalr
    40, // INSTRUCTION: 0b011000 => slladd
    43, // INSTRUCTION: 0b011001 => srladd
    46, // INSTRUCTION: 0b011010 => sraadd
    49, // INSTRUCTION: 0b011011 => roradd
    52, // INSTRUCTION: 0b011100 => sllsub
    55, // INSTRUCTION: 0b011101 => srlsub
    58, // INSTRUCTION: 0b011110 => srasub
    61, // INSTRUCTION: 0b011111 => rorsub
    64, // INSTRUCTION: 0b100000 => lw
    67, // INSTRUCTION: 0b100001 => sw
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    69, // INSTRUCTION: 0b110000 => addm
    77, // INSTRUCTION: 0b110001 => subm
    -1, // INSTRUCTION: 0b110010 => orm
    -1, // INSTRUCTION: 0b110011 => andm
    73, // INSTRUCTION: 0b110100 => addm
    81, // INSTRUCTION: 0b110101 => subm
    -1, // INSTRUCTION: 0b110110 => orm
    -1, // INSTRUCTION: 0b110111 => andm
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // INSTRUCTION: 0b111100 => addmi
    -1, // NOTHING
    -1, // INSTRUCTION: 0b111110 => ormi
};

// Translates an opcode to the index/label of the implemented instruction in the
// MICROCODE
int opcode_to_microcode_idx(int opcode) {
  if (opcode >= JUMP_TABLE_SIZE) {
    printf("opcode exceeds %d.\n", JUMP_TABLE_SIZE);
    exit(EXIT_FAILURE);
  }

  int microcode_idx = OPCODE_JUMP_TABLE[opcode];

  if (microcode_idx < 0) {
    printf("Unimplemented opcode %d\n", opcode);
    exit(EXIT_FAILURE);
  }

  return microcode_idx;
}
