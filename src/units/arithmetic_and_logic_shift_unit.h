#pragma once

#include "../multiplexers.h"

enum ALSUOp {
    ALSUOp_Sll,
    ALSUOp_Srl,
    ALSUOp_Sra,
    ALSUOp_Ror,
};

unsigned int alsu(unsigned int num, unsigned int amount, enum ALSUOp op);
