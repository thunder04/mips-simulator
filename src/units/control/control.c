#include "control.h"
#include "../../special_function_registers.h"
#include "microcode.h"

// Instruction Counter and μPC
unsigned int IC = 0, microPC = 0;
// CPU Clock
unsigned int clock = 0;

void control() {
  while (1) {
    struct MicrocodeRow microInstruction = MICROCODE[microPC];

    // Increment the CPU clock
    clock += 1;

    // Handle microcode columns
    // NOTE: I could use a switch/case here, but it turns out it's more verbose
    // than a plain if/else chain.

    // clang-format off

    // Handle the ALU column
    // ALU1 and ALU2 are provided as is by the μInstruction
    enum ALUOpSel ALUOp;
    
    if (microInstruction.alu == mACadd) ALUOp = ALUOp_Add;
    else if (microInstruction.alu == mACsub) ALUOp = ALUOp_Sub;
    else if (microInstruction.alu == mACand) ALUOp = ALUOp_And;
    else if (microInstruction.alu == mACor) ALUOp = ALUOp_Or;

    // Handle the ALSU column
    // ALSU1 and ALSU2 are provided as is by the μInstruction
    enum ALSUOpSel ALSUOp;
    
    if (microInstruction.alsu == mASCsll) ALSUOp = ALSUOp_Sll;
    else if (microInstruction.alsu == mASCsrl) ALSUOp = ALSUOp_Srl;
    else if (microInstruction.alsu == mASCsra) ALSUOp = ALSUOp_Sra;
    else if (microInstruction.alsu == mASCror) ALSUOp = ALSUOp_Ror;

    // clang-format on

    // Handle the RF column
    unsigned int ReadRegister1 = 0, ReadRegister2 = 0, WriteRegister = 0,
                 RfWriteData = 0, RegWrite = 0;

    switch (microInstruction.rf) {
    case mRCread_rs_rt:
      ReadRegister1 = IR_rs();
      ReadRegister2 = IR_rt();
      break;
    case mRCread_rs_rd:
      ReadRegister1 = IR_rs();
      ReadRegister2 = IR_rd();
      break;
    case mRCwrite_c:
      WriteRegister = IR_rd();
      RfWriteData = C;
      break;
    case mRCwrite_d:
      WriteRegister = IR_rd();
      RfWriteData = D;
      break;
    case mRCwrite_dr:
      WriteRegister = IR_rt();
      RfWriteData = DR;
      break;
    case mRCwrite_ra_a:
      WriteRegister = 31;
      RfWriteData = A;
      break;
    case mRCwrite_ra_c:
      WriteRegister = 31;
      RfWriteData = C;
      break;
    case mRCnothing:
      break;
    }

    // Handle the MEM column
    unsigned int Address = 0, MemRead = 0, MemWrite = 0, MemWriteData = 0;
    switch (microInstruction.mem) {
    case mMCread_pc:
      MemRead = 1;
      Address = PC;
      break;
    case mMCread_c:
      MemRead = 1;
      Address = C;
      break;
    case mMCwrite_c:
      MemWrite = 1;
      MemWriteData = B;
      Address = C;
      break;
    case mMCnothing:
      break;
    }

    // Handle the sequencing column
    switch (microInstruction.sequencing.kind) {
    case mSKseq:
      microPC += 1;
      break;
    case mSKlabel:
      microPC = microInstruction.sequencing.label;
      break;
    case mSKdisp:
      microPC = opcode_to_microcode_idx(IR_opcode());
      // It seems a good place to increment the Instruction Counter
      IC += 1;
      break;
    }
  }
}
