#include "microcode.h"
#include <stdio.h>
#include <stdlib.h>

#define JUMP_TABLE_SIZE 63

// Be mindful when you change the order of the microcode. Only append to the
// end. If you end up needing an extra cycle for an already implemented
// instruction, do NOT forget to update the jump table and the comments.
// clang-format off
const struct MicrocodeRow MICROCODE[] = {
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
  { .sequencing = { mSKseq }, .alsu = mASCsll, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_A },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 13 - srlv: rd = rt >> rs
  { .sequencing = { mSKseq }, .alsu = mASCsrl, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_A },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 15 - srav: rd = rt >>> rs
  { .sequencing = { mSKseq }, .alsu = mASCsra, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_A },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 17 - rorv: rd = rt ROR rs
  { .sequencing = { mSKseq }, .alsu = mASCror, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_A },
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
  { .sequencing = { mSKseq }, .alsu = mASCsll, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 27 - srl: rd = rt >> shamt
  { .sequencing = { mSKseq }, .alsu = mASCsrl, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 29 - sra: rd = rt >>> shamt
  { .sequencing = { mSKseq }, .alsu = mASCsra, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_d },
  // 31 - ror: rd = rt ROR shamt
  { .sequencing = { mSKseq }, .alsu = mASCror, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_Shamt },
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
  { .sequencing = { mSKseq }, .alsu = mASCsll, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 43 - srladd: D = rt >> shamt; rd = rs + D
  { .sequencing = { mSKseq }, .alsu = mASCsrl, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 46 - sraadd: D = rt >>> shamt; rd = rs + D
  { .sequencing = { mSKseq }, .alsu = mASCsra, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 49 - roradd: D = rt ROR shamt; rd = rs + D
  { .sequencing = { mSKseq }, .alsu = mASCror, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 52 - sllsub: D = rt << shamt; rd = rs - D
  { .sequencing = { mSKseq }, .alsu = mASCsll, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 55 - srlsub: D = rt >> shamt; rd = rs - D
  { .sequencing = { mSKseq }, .alsu = mASCsrl, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 58 - srasub: D = rt >>> shamt; rd = rs - D
  { .sequencing = { mSKseq }, .alsu = mASCsra, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 61 - rorsub: D = rt ROR shamt; rd = rs - D
  { .sequencing = { mSKseq }, .alsu = mASCror, .alsu1 = ALSUSrcA_B, .alsu2 = ALSUSrcB_Shamt },
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_D },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
};
// clang-format on

// Do NOT change the order of the jump table. It relies on index position
// Auto-generated by gen_jump_table.py
static int OPCODE_JUMP_TABLE[JUMP_TABLE_SIZE] = {
    3,  // INSTRUCTION: add
    5,  // INSTRUCTION: sub
    7,  // INSTRUCTION: or
    9,  // INSTRUCTION: and
    11, // INSTRUCTION: sllv
    13, // INSTRUCTION: srlv
    15, // INSTRUCTION: srav
    17, // INSTRUCTION: rorv
    19, // INSTRUCTION: addi
    -1, // NOTHING
    21, // INSTRUCTION: ori
    23, // INSTRUCTION: andi
    25, // INSTRUCTION: sll
    27, // INSTRUCTION: srl
    29, // INSTRUCTION: sra
    31, // INSTRUCTION: ror
    33, // INSTRUCTION: beq
    34, // INSTRUCTION: bne
    -1, // NOTHING
    2,  // INSTRUCTION: break
    35, // INSTRUCTION: j
    36, // INSTRUCTION: jr
    37, // INSTRUCTION: jal
    38, // INSTRUCTION: jalr
    40, // INSTRUCTION: slladd
    43, // INSTRUCTION: srladd
    46, // INSTRUCTION: sraadd
    49, // INSTRUCTION: roradd
    52, // INSTRUCTION: sllsub
    55, // INSTRUCTION: srlsub
    58, // INSTRUCTION: srasub
    61, // INSTRUCTION: rorsub
    -1, // INSTRUCTION: lw
    -1, // INSTRUCTION: sw
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
    -1, // INSTRUCTION: addm
    -1, // INSTRUCTION: subm
    -1, // INSTRUCTION: orm
    -1, // INSTRUCTION: andm
    -1, // INSTRUCTION: addm
    -1, // INSTRUCTION: subm
    -1, // INSTRUCTION: orm
    -1, // INSTRUCTION: andm
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // NOTHING
    -1, // INSTRUCTION: addmi
    -1, // NOTHING
    -1, // INSTRUCTION: ormi
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
