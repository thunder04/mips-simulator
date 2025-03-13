#include "stats.h"
#include "../../special_function_registers.h"
#include <stdio.h>

// Calculated from "src/units/control/microcode.c"
const int CYCLES_PER_INSTRUCTION_CATEGORY[InstructionCategoryLen] = {
    [ICalu] = 4,     [ICalsu] = 4,      [ICalu_alsu] = 5, [ICmem] = 5,
    [ICmem_alu] = 6, [ICcond_jump] = 3, [ICjump] = 3,     [ICbreak] = 3,
};

// An array keeping track of instruction counters, to calculate the CPI at the
// end
unsigned int ICStats[InstructionCategoryLen] = {0};

unsigned int calculate_cpi() {
    unsigned int instruction_count = 0;
    double cpi = 0;

    for (int i = 0; i < InstructionCategoryLen; ++i) {
        instruction_count += ICStats[i];
    }

    if (instruction_count == 0) {
        return 0;
    }

    for (int i = 0; i < InstructionCategoryLen; ++i) {
        cpi += ((double)ICStats[i] / instruction_count) *
               CYCLES_PER_INSTRUCTION_CATEGORY[i];
    }

    return (unsigned int)cpi;
}

void handle_ic_stats() {
    switch (IR_opcode()) {
    case 0b000000: // add
    case 0b000001: // sub
    case 0b000010: // or
    case 0b000011: // and
    case 0b001000: // addi
    case 0b001010: // ori
    case 0b001011: // andi
        ICStats[ICalu] += 1;
        break;
    case 0b000100: // sllv
    case 0b000101: // srlv
    case 0b000110: // srav
    case 0b000111: // rorv
    case 0b001100: // sll
    case 0b001101: // srl
    case 0b001110: // sra
    case 0b001111: // ror
        ICStats[ICalsu] += 1;
        break;
    case 0b010000: // beq
    case 0b010001: // bne
        ICStats[ICcond_jump] += 1;
        break;
    case 0b010011: // break
        ICStats[ICbreak] += 1;
        break;
    case 0b010100: // j
    case 0b010101: // jr
    case 0b010110: // jal
    case 0b010111: // jalr
        ICStats[ICjump] += 1;
        break;
    case 0b011000: // slladd
    case 0b011001: // srladd
    case 0b011010: // sraadd
    case 0b011011: // roradd
    case 0b011100: // sllsub
    case 0b011101: // srlsub
    case 0b011110: // srasub
    case 0b011111: // rorsub
        ICStats[ICalu_alsu] += 1;
        break;
    case 0b100000: // lw
    case 0b100001: // sw
        ICStats[ICmem] += 1;
        break;
    case 0b110000: // addm
    case 0b110001: // subm
    case 0b110010: // orm
    case 0b110011: // andm
    case 0b110100: // addm
    case 0b110101: // subm
    case 0b110110: // orm
    case 0b110111: // andm
    case 0b111100: // addmi
    case 0b111110: // ormi
    case 0b111111: // andmi
        ICStats[ICmem_alu] += 1;
        break;
    }
}
