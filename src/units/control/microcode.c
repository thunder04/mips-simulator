#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"

#include "microcode.h"
#include <stdio.h>
#include <stdlib.h>

#define JUMP_TABLE_SIZE 64

// Be mindful when you change the order of the microcode. Only append to the
// end. If you end up needing an extra cycle for an already implemented
// instruction, do NOT forget to update the jump table, the index in the
// comments and the `./stats.c` file.

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
  // 85 - orm: DR = MEM[rs + imm]; rt = rt | DR
  { .sequencing = { mSKseq }, .alu = mACor, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKseq }, .mem = mMCread_c },
  { .sequencing = { mSKseq }, .alu = mACor, .alu1 = ALUSrcA_B, .alu2 = ALUSrcB_DR },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rt_c },
  // 89 - orm: A = MEM[rs]; B = MEM[rt]; rd = A | B
  { .sequencing = { mSKseq }, .mem = mMCread_a },
  { .sequencing = { mSKseq }, .mem = mMCread_b },
  { .sequencing = { mSKseq }, .alu = mACor, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 93 - andm: DR = MEM[rs + imm]; rt = rt & DR
  { .sequencing = { mSKseq }, .alu = mACand, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKseq }, .mem = mMCread_c },
  { .sequencing = { mSKseq }, .alu = mACand, .alu1 = ALUSrcA_B, .alu2 = ALUSrcB_DR },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rt_c },
  // 97 - andm: A = MEM[rs]; B = MEM[rt]; rd = A & B
  { .sequencing = { mSKseq }, .mem = mMCread_a },
  { .sequencing = { mSKseq }, .mem = mMCread_b },
  { .sequencing = { mSKseq }, .alu = mACand, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_B },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rd_c },
  // 101 - addmi: A = MEM[rs]; rt = A + imm
  { .sequencing = { mSKseq }, .mem = mMCread_a },
  { .sequencing = { mSKseq }, .alu = mACadd, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rt_c },
  // 104 - ormi: A = MEM[rs]; rt = A | imm
  { .sequencing = { mSKseq }, .mem = mMCread_a },
  { .sequencing = { mSKseq }, .alu = mACor, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rt_c },
  // 107 - andmi: A = MEM[rs]; rt = A & imm
  { .sequencing = { mSKseq }, .mem = mMCread_a },
  { .sequencing = { mSKseq }, .alu = mACand, .alu1 = ALUSrcA_A, .alu2 = ALUSrcB_SignExtend },
  { .sequencing = { mSKlabel, 0 }, .rf = mRCwrite_rt_c },
};

// clang-format on

// Auto-generated by gen_jump_table.py
static int OPCODE_JUMP_TABLE[JUMP_TABLE_SIZE] = {
    [0b000000] = 3,   // add
    [0b000001] = 5,   // sub
    [0b000010] = 7,   // or
    [0b000011] = 9,   // and
    [0b000100] = 11,  // sllv
    [0b000101] = 13,  // srlv
    [0b000110] = 15,  // srav
    [0b000111] = 17,  // rorv
    [0b001000] = 19,  // addi
    [0b001010] = 21,  // ori
    [0b001011] = 23,  // andi
    [0b001100] = 25,  // sll
    [0b001101] = 27,  // srl
    [0b001110] = 29,  // sra
    [0b001111] = 31,  // ror
    [0b010000] = 33,  // beq
    [0b010001] = 34,  // bne
    [0b010011] = 2,   // break
    [0b010100] = 35,  // j
    [0b010101] = 36,  // jr
    [0b010110] = 37,  // jal
    [0b010111] = 38,  // jalr
    [0b011000] = 40,  // slladd
    [0b011001] = 43,  // srladd
    [0b011010] = 46,  // sraadd
    [0b011011] = 49,  // roradd
    [0b011100] = 52,  // sllsub
    [0b011101] = 55,  // srlsub
    [0b011110] = 58,  // srasub
    [0b011111] = 61,  // rorsub
    [0b100000] = 64,  // lw
    [0b100001] = 67,  // sw
    [0b110000] = 69,  // addm
    [0b110001] = 77,  // subm
    [0b110010] = 85,  // orm
    [0b110011] = 93,  // andm
    [0b110100] = 73,  // addm
    [0b110101] = 81,  // subm
    [0b110110] = 89,  // orm
    [0b110111] = 97,  // andm
    [0b111100] = 101, // addmi
    [0b111110] = 104, // ormi
    [0b111111] = 107, // andmi
};

// Translates an opcode to the index/label of the implemented instruction in
// the MICROCODE
int opcode_to_microcode_idx(int opcode) {
    if (opcode >= JUMP_TABLE_SIZE) {
        printf("opcode exceeds %d.\n", JUMP_TABLE_SIZE);
        exit(EXIT_FAILURE);
    }

    int microcode_idx = OPCODE_JUMP_TABLE[opcode];

    if (microcode_idx < 1) {
        printf("Unimplemented opcode %d\n", opcode);
        exit(EXIT_FAILURE);
    }

    return microcode_idx;
}

#pragma clang diagnostic pop
