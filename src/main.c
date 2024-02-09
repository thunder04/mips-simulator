// Multi-cycle MIPS Processor Simulator

#include "macros.h"
#include "microcode.h"
#include "special_function_registers.h"
#include "units/memory.h"
#include "units/register_file.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// +2 to account for \0 and \n
#define LINE_LIMIT 32 + 2

// Instruction Counter and μPC
static unsigned int IC = 0, microPC = 0;
// CPU Clock
static unsigned int clock = 0;

void parse_file(FILE *fptr);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <object-file-path>\n", argv[0]);
    return 1;
  }

  FILE *fptr = fopen(argv[1], "r");

  if (fptr == NULL) {
    printf("Failed to read the pseudo-object file: %s\n", strerror(errno));
    return 2;
  }

  // Parse and close the file as it's not needed anymore
  parse_file(fptr);
  fclose(fptr);

  // TODO Move me to a units/control folder, along with the microcode.{h,c}
  // Run the Control unit
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

  return 0;
}

void parse_file(FILE *fptr) {
  // Get file's size
  fseek(fptr, 0, SEEK_END);
  long flen = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  if (flen > MEM_MAX_PROGRAM_SIZE) {
    printf("The file exceeds %d bytes\n", MEM_MAX_PROGRAM_SIZE);
    exit(4);
  }

  // Initialize PC from the end of reserved space, according to the textbook.
  PC = MEM_RESERVED_SPACE;

  char buf[LINE_LIMIT];
  unsigned int addr = PC;

  while (fgets(buf, LINE_LIMIT, fptr)) {
    char *endptr;
    unsigned int word = (unsigned int)strtol(buf, &endptr, 2);

    if (*endptr != '\0' && *endptr != '\n' && *endptr != '\r') {
#ifdef DEBUG
      printf(ANSI_FM "[DEBUG] Skipping line \"%s\"\n" ANSI_0, buf);
#endif

      continue;
    }

    mem(addr, 0, 1, word);
    addr += 4;
  }
}
