#include "microcode.h"
#include <stdio.h>
#include <stdlib.h>

#define JUMP_TABLE_SIZE 63

// clang-format off
const struct MicrocodeRow MICROCODE[] = {
  // 0 - fetch:
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_PC, .alu2 = ALUSrcB_4, .mem = mMCread_pc, .pc = mPWCalu_out },
  { .sequencing = { mSKdisp }, .alu = mACadd, .alu1 = ALUSrcA_PC, .alu2 = ALUSrcB_SignExtend, .rf = mRCread },
  // 2 - break:
  { .sequencing = { mSKlabel, 0 }, .exit = mECexit },
  // 3 - add:
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_c },
  // 5 - sub:
  { .sequencing = { mSKseq }, .alu = mACsub, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_c },
  // 7 - or:
  { .sequencing = { mSKseq }, .alu = mACor, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_c },
  // 9 - and:
  { .sequencing = { mSKseq }, .alu = mACand, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_c },
  // 11 - sllv:
  { .sequencing = { mSKseq }, .alsu = mASCsll, .alsu1 = ALSUSrcA_A, .alsu2 = ALSUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_d },
  // 13 - srlv:
  { .sequencing = { mSKseq }, .alsu = mASCsrl, .alsu1 = ALSUSrcA_A, .alsu2 = ALSUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_d },
  // 15 - srav:
  { .sequencing = { mSKseq }, .alsu = mASCsra, .alsu1 = ALSUSrcA_A, .alsu2 = ALSUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_d },
  // 17 - rorv:
  { .sequencing = { mSKseq }, .alsu = mASCror, .alsu1 = ALSUSrcA_A, .alsu2 = ALSUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_d },
  // 19 - addi:
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_c },
  // 21 - ori:
  { .sequencing = { mSKseq }, .alu = mACor, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_c },
  // 23 - andi:
  { .sequencing = { mSKseq }, .alu = mACand, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_c },
  // 25 - sll:
  { .sequencing = { mSKseq }, .alsu = mASCsll, .alsu1 = ALSUSrcA_A, .alsu2 = ALSUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_d },
  // 27 - srl:
  { .sequencing = { mSKseq }, .alsu = mASCsrl, .alsu1 = ALSUSrcA_A, .alsu2 = ALSUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_d },
  // 29 - sra:
  { .sequencing = { mSKseq }, .alsu = mASCsra, .alsu1 = ALSUSrcA_A, .alsu2 = ALSUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_d },
  // 31 - ror:
  { .sequencing = { mSKseq }, .alsu = mASCror, .alsu1 = ALSUSrcA_A, .alsu2 = ALSUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_d },
};
// clang-format on

static int OPCODE_JUMP_TABLE[JUMP_TABLE_SIZE] = {
    3, // INSTRUCTION: add
    5, // INSTRUCTION: sub
    7, // INSTRUCTION: or
    9, // INSTRUCTION: and
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
    -1, // INSTRUCTION: beq
    -1, // INSTRUCTION: bne
    -1, // NOTHING
    2,  // INSTRUCTION: break
    -1, // INSTRUCTION: j
    -1, // INSTRUCTION: jr
    -1, // INSTRUCTION: jal
    -1, // INSTRUCTION: jalr
    -1, // INSTRUCTION: slladd
    -1, // INSTRUCTION: srladd
    -1, // INSTRUCTION: sraadd
    -1, // INSTRUCTION: roradd
    -1, // INSTRUCTION: sllsub
    -1, // INSTRUCTION: srlsub
    -1, // INSTRUCTION: srasub
    -1, // INSTRUCTION: rorsub
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

// Translates an opcode to the index/label of the implemented instruction in the MICROCODE
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
