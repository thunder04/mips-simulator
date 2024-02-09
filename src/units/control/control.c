#include "control.h"
#include "../../macros.h"
#include "../../multiplexers.h"
#include "../../special_function_registers.h"
#include "../arithmetic_and_logic_shift_unit.h"
#include "../arithmetic_and_logic_unit.h"
#include "../memory.h"
#include "../register_file.h"
#include "microcode.h"
#include <stdio.h>

const struct RegOut EMPTY_REG_RETURN = {0, 0};
const struct ALUOut EMPTY_ALU_RETURN = {1, 0};

// Instruction Counter and μPC
unsigned int IC = 0, microPC = 0;
// CPU Clock
unsigned int clock = 0;

void handle_alu_column(struct MicrocodeRow *microIns, enum ALUOp *ALUOp);
void handle_alsu_column(struct MicrocodeRow *microIns, enum ALSUOp *ALSUOp);
void handle_rf_column(struct MicrocodeRow *microIns,
                      unsigned int *ReadRegister1, unsigned int *ReadRegister2,
                      unsigned int *RegWrite, enum RegDstSel *RegDstSel,
                      enum MemToRegSel *MemToRegSel);
void handle_mem_column(struct MicrocodeRow *microIns, unsigned int *MemRead,
                       unsigned int *MemWrite, enum IorDSel *IorDSel);
void handle_sequencing_column(struct MicrocodeRow *microIns);

void control() {
  while (1) {
    struct MicrocodeRow microIns = MICROCODE[microPC];

    // Increment the CPU clock
    clock += 1;

    // Handle the ALU column
    // ALU1 and ALU2 are provided as is by the μInstruction for simplicity
    enum ALUOp ALUOp;

    handle_alu_column(&microIns, &ALUOp);

    // Handle the ALSU column
    // ALSU1 and ALSU2 are provided as is by the μInstruction for simplicity
    enum ALSUOp ALSUOp;

    handle_alsu_column(&microIns, &ALSUOp);

    // Handle the RF column
    unsigned int ReadRegister1 = 0, ReadRegister2 = 0, RegWrite = 0;
    enum MemToRegSel MemToRegSel;
    enum RegDstSel RegDstSel;

    handle_rf_column(&microIns, &ReadRegister1, &ReadRegister2, &RegWrite,
                     &RegDstSel, &MemToRegSel);

    // Handle the MEM column
    unsigned int MemRead = 0, MemWrite = 0;
    enum IorDSel IorDSel;

    handle_mem_column(&microIns, &MemRead, &MemWrite, &IorDSel);

    // Handle the sequencing column
    handle_sequencing_column(&microIns);

    // Call the units

    MemData MemOut = microIns.mem != mMCnothing
                         ? mem(IorD(IorDSel), MemRead, MemWrite, B)
                         : 0;

    struct RegOut RegOut =
        microIns.rf != mRCnothing
            ? reg(ReadRegister1, ReadRegister2, RegDst(RegDstSel),
                  MemToReg(MemToRegSel), RegWrite)
            : EMPTY_REG_RETURN;

    struct ALUOut ALUOut =
        microIns.alu != mACnothing
            ? alu(ALUSrcA(microIns.alu1), ALUSrcB(microIns.alu2), ALUOp)
            : EMPTY_ALU_RETURN;

    unsigned int ALSUOut =
        microIns.alsu != mASCnothing
            ? alsu(ALSUSrcA(microIns.alsu1), ALSUSrcB(microIns.alsu2), ALSUOp)
            : 0;

    // Perhaps a bit late, but better than never. Handle the PC column
    enum PCSrcSel PCSrcSel = PCSrc_C;

    if (microIns.pc == mPWCa)
      PCSrcSel = PCSrc_A;
    else if (microIns.pc == mPWCb)
      PCSrcSel = PCSrc_B;
    else if (microIns.pc == mPWCalu_out)
      PCSrcSel = PCSrc_ALUOut;
    else if (microIns.pc == mPWCjump_address)
      PCSrcSel = PCSrc_IRAddr;

    // Write to PC only if the write signal is enabled OR either the c_cond or
    // c_not_cond signals are enabled and their conditions is met
    if (((microIns.pc == mPWCc_cond && ALUOut.Zero) ||
         (microIns.pc == mPWCc_not_cond && !ALUOut.Zero)) ||
        microIns.pc != mPWCnothing) {
      PC = PCSrc(PCSrcSel, ALUOut.Out);
    }

    // Write to registers

    if (microIns.mem == mMCread_pc)
      DR = IR = MemOut;
    else if (microIns.mem == mMCread_c)
      DR = MemOut;

    if (microIns.rf != mRCnothing) {
      A = RegOut.ReadData1;
      B = RegOut.ReadData2;
    }

    if (microIns.alu != mACnothing)
      C = ALUOut.Out;
    if (microIns.alsu != mASCnothing)
      D = ALSUOut;

    // Exit simulation if we received the break instruction
    if (microIns.exit == mECexit) {
      break;
    }
  }
}

void handle_sequencing_column(struct MicrocodeRow *microIns) {
  switch (microIns->sequencing.kind) {
  case mSKseq:
    microPC += 1;
    break;
  case mSKlabel:
    microPC = microIns->sequencing.label;
    break;
  case mSKdisp:
    microPC = opcode_to_microcode_idx(IR_opcode());
    // Here it seems a good place to increment the Instruction Counter
    IC += 1;

#ifdef DEBUG
    printf(ANSI_FM "[DEBUG] About to process instruction: " ANSI_FR);

    // Print the instruction in bits
    for (int b = 0, l = sizeof(unsigned int) * 8; b < l; ++b) {
      printf("%i", (IR >> (l - b - 1)) & 0x01);

      if (b > 0 && b % 5 == 0 && b < 30)
        printf(" ");
    }

    printf("\n" ANSI_0);
#endif

    break;
  }
}

void handle_alu_column(struct MicrocodeRow *microIns, enum ALUOp *ALUOp) {
  if (microIns->alu == mACadd)
    *ALUOp = ALUOp_Add;
  else if (microIns->alu == mACsub)
    *ALUOp = ALUOp_Sub;
  else if (microIns->alu == mACand)
    *ALUOp = ALUOp_And;
  else if (microIns->alu == mACor)
    *ALUOp = ALUOp_Or;
}

void handle_alsu_column(struct MicrocodeRow *microIns, enum ALSUOp *ALSUOp) {
  if (microIns->alsu == mASCsll)
    *ALSUOp = ALSUOp_Sll;
  else if (microIns->alsu == mASCsrl)
    *ALSUOp = ALSUOp_Srl;
  else if (microIns->alsu == mASCsra)
    *ALSUOp = ALSUOp_Sra;
  else if (microIns->alsu == mASCror)
    *ALSUOp = ALSUOp_Ror;
}

void handle_rf_column(struct MicrocodeRow *microIns,
                      unsigned int *ReadRegister1, unsigned int *ReadRegister2,
                      unsigned int *RegWrite, enum RegDstSel *RegDstSel,
                      enum MemToRegSel *MemToRegSel) {
  switch (microIns->rf) {
  case mRCread_rs_rt:
    *ReadRegister1 = IR_rs();
    *ReadRegister2 = IR_rt();
    break;
  case mRCread_rs_rd:
    *ReadRegister1 = IR_rs();
    *ReadRegister2 = IR_rd();
    break;
  case mRCwrite_rd_c:
    *RegDstSel = RegDst_Rd;
    *MemToRegSel = MemToReg_C;
    *RegWrite = 1;
    break;
  case mRCwrite_rd_d:
    *RegDstSel = RegDst_Rd;
    *MemToRegSel = MemToReg_D;
    *RegWrite = 1;
    break;
  case mRCwrite_rt_c:
    *RegDstSel = RegDst_Rt;
    *MemToRegSel = MemToReg_C;
    *RegWrite = 1;
    break;
  case mRCwrite_rt_d:
    *RegDstSel = RegDst_Rt;
    *MemToRegSel = MemToReg_D;
    *RegWrite = 1;
    break;
  case mRCwrite_rt_dr:
    *RegDstSel = RegDst_Rt;
    *MemToRegSel = MemToReg_DR;
    *RegWrite = 1;
    break;
  case mRCwrite_ra_a:
    *RegDstSel = RegDst_Ra;
    *MemToRegSel = MemToReg_A;
    *RegWrite = 1;
    break;
  case mRCwrite_ra_b:
    *RegDstSel = RegDst_Ra;
    *MemToRegSel = MemToReg_B;
    *RegWrite = 1;
    break;
  case mRCwrite_ra_pc:
    *RegDstSel = RegDst_Ra;
    *MemToRegSel = MemToReg_PC;
    *RegWrite = 1;
    break;
  case mRCnothing:
    break;
  }
}

void handle_mem_column(struct MicrocodeRow *microIns, unsigned int *MemRead,
                       unsigned int *MemWrite, enum IorDSel *IorDSel) {
  switch (microIns->mem) {
  case mMCread_pc:
    *MemRead = 1;
    *IorDSel = IorD_PC;
    break;
  case mMCread_c:
    *MemRead = 1;
    *IorDSel = IorD_C;
    break;
  case mMCwrite_c:
    *MemWrite = 1;
    *IorDSel = IorD_C;
    break;
  case mMCnothing:
    break;
  }
}
