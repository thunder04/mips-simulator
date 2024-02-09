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
const struct ALUOut EMPTY_ALU_RETURN = {0, 0}; // Setting Zero=1 is intentional

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

    // Call the units and assign their return values to the respective variables
    // and registers
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
        microIns.alu != mASCnothing
            ? alsu(ALSUSrcA(microIns.alsu1), ALSUSrcB(microIns.alsu2), ALSUOp)
            : 0;

    // TODO Write to PC?

    // Write to registers
    A = RegOut.ReadData1;
    B = RegOut.ReadData2;

    if (microIns.exit == mECexit) {
      printf(ANSI_FM "Exiting simulation\n" ANSI_0);
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
  case mRCwrite_c:
    *RegDstSel = RegDst_Rd;
    *MemToRegSel = MemToReg_C;
    break;
  case mRCwrite_d:
    *RegDstSel = RegDst_Rd;
    *MemToRegSel = MemToReg_D;
    break;
  case mRCwrite_dr:
    *RegDstSel = RegDst_Rt;
    *MemToRegSel = MemToReg_DR;
    break;
  case mRCwrite_ra_a:
    *RegDstSel = RegDst_Ra;
    *MemToRegSel = MemToReg_A;
    break;
  case mRCwrite_ra_c:
    *RegDstSel = RegDst_Ra;
    *MemToRegSel = MemToReg_C;
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
