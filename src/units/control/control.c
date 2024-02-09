#include "control.h"
#include "../../special_function_registers.h"
#include "../arithmetic_and_logic_shift_unit.h"
#include "../arithmetic_and_logic_unit.h"
#include "../memory.h"
#include "../register_file.h"
#include "microcode.h"

// Instruction Counter and μPC
unsigned int IC = 0, microPC = 0;
// CPU Clock
unsigned int clock = 0;

void handle_alu_column(struct MicrocodeRow *microIns, enum ALUOpSel *ALUOp);
void handle_alsu_column(struct MicrocodeRow *microIns, enum ALSUOpSel *ALSUOp);
void handle_rf_column(struct MicrocodeRow *microIns,
                      unsigned int *ReadRegister1, unsigned int *ReadRegister2,
                      unsigned int *WriteRegister, unsigned int *WriteData,
                      unsigned int *RegWrite);
void handle_mem_column(struct MicrocodeRow *microIns, unsigned int *Address,
                       unsigned int *MemRead, unsigned int *MemWrite,
                       unsigned int *WriteData);
void handle_sequencing_column(struct MicrocodeRow *microIns);

void control() {
  while (1) {
    struct MicrocodeRow microIns = MICROCODE[microPC];

    // Increment the CPU clock
    clock += 1;

    // Handle the ALU column
    // ALU1 and ALU2 are provided as is by the μInstruction for simplicity
    enum ALUOpSel ALUOp;
    handle_alu_column(&microIns, &ALUOp);

    // Handle the ALSU column
    // ALSU1 and ALSU2 are provided as is by the μInstruction for simplicity
    enum ALSUOpSel ALSUOp;
    handle_alsu_column(&microIns, &ALSUOp);

    // Handle the RF column
    unsigned int ReadRegister1 = 0, ReadRegister2 = 0, WriteRegister = 0,
                 RfWriteData = 0, RegWrite = 0;
    handle_rf_column(&microIns, &ReadRegister1, &ReadRegister2, &WriteRegister,
                     &RfWriteData, &RegWrite);

    // Handle the MEM column
    unsigned int Address = 0, MemRead = 0, MemWrite = 0, MemWriteData = 0;
    handle_mem_column(&microIns, &Address, &MemRead, &MemWrite, &MemWriteData);

    // Handle the sequencing column
    handle_sequencing_column(&microIns);
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
    // It seems a good place to increment the Instruction Counter
    IC += 1;
    break;
  }
}

void handle_alu_column(struct MicrocodeRow *microIns, enum ALUOpSel *ALUOp) {
  if (microIns->alu == mACadd)
    *ALUOp = ALUOp_Add;
  else if (microIns->alu == mACsub)
    *ALUOp = ALUOp_Sub;
  else if (microIns->alu == mACand)
    *ALUOp = ALUOp_And;
  else if (microIns->alu == mACor)
    *ALUOp = ALUOp_Or;
}

void handle_alsu_column(struct MicrocodeRow *microIns, enum ALSUOpSel *ALSUOp) {
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
                      unsigned int *WriteRegister, unsigned int *WriteData,
                      unsigned int *RegWrite) {
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
    *WriteRegister = IR_rd();
    *WriteData = C;
    break;
  case mRCwrite_d:
    *WriteRegister = IR_rd();
    *WriteData = D;
    break;
  case mRCwrite_dr:
    *WriteRegister = IR_rt();
    *WriteData = DR;
    break;
  case mRCwrite_ra_a:
    *WriteRegister = 31;
    *WriteData = A;
    break;
  case mRCwrite_ra_c:
    *WriteRegister = 31;
    *WriteData = C;
    break;
  case mRCnothing:
    break;
  }
}

void handle_mem_column(struct MicrocodeRow *microIns, unsigned int *Address,
                       unsigned int *MemRead, unsigned int *MemWrite,
                       unsigned int *WriteData) {
  switch (microIns->mem) {
  case mMCread_pc:
    *MemRead = 1;
    *Address = PC;
    break;
  case mMCread_c:
    *MemRead = 1;
    *Address = C;
    break;
  case mMCwrite_c:
    *MemWrite = 1;
    *WriteData = B;
    *Address = C;
    break;
  case mMCnothing:
    break;
  }
}
