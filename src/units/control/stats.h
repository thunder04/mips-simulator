#pragma once

enum InstructionCategory {
    ICalu,
    ICalsu,
    ICalu_alsu,
    ICmem,
    ICmem_alu,
    ICcond_jump,
    ICjump,
    ICbreak,
    // Keep this to the end
    InstructionCategoryLen
};

extern const int CYCLES_PER_INSTRUCTION_CATEGORY[InstructionCategoryLen];
extern unsigned int ICStats[InstructionCategoryLen];

unsigned int calculate_cpi();
void handle_ic_stats();
