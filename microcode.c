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
};
// clang-format on

static int OPCODE_JUMP_TABLE[JUMP_TABLE_SIZE] = {
    -1, // INSTRUCTION: add
    -1, // INSTRUCTION: sub
    -1, // INSTRUCTION: or
    -1, // INSTRUCTION: and
    -1, // INSTRUCTION: sllv
    -1, // INSTRUCTION: srlv
    -1, // INSTRUCTION: srav
    -1, // INSTRUCTION: rorv
    -1, // INSTRUCTION: addi
    -1, // NOTHING
    -1, // INSTRUCTION: ori
    -1, // INSTRUCTION: andi
    -1, // INSTRUCTION: sll
    -1, // INSTRUCTION: srl
    -1, // INSTRUCTION: sra
    -1, // INSTRUCTION: ror
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
