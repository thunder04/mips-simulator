#include "arithmetic_and_logic_shift_unit.h"
#include "../macros.h"

unsigned int alsu(unsigned int num, unsigned int amount, enum ALSUOp op) {
    switch (op) {
    case ALSUOp_Sll:
        return num << amount;
    case ALSUOp_Srl:
        return num >> amount;
    case ALSUOp_Sra:
        if ((int)num < 0) {
            // SRA on signed integers is compiler dependent behavior.
            // Sometimes it's even undefined behavior, therefore it's
            // unreliable.
            //
            // Source: https://stackoverflow.com/a/53766752
            return (unsigned int)(~(~(int)num >> amount));
        } else {
            return num >> amount;
        }
    case ALSUOp_Ror:
        // num >> amount: Make room for the new digits
        //
        // num & MASK(amount) = RES: Isolate the bits to be moved.
        //
        // RES << (32 - amount): Place the bits to the MSB position.
        return (num >> amount) | ((num & MASK(amount)) << (32 - amount));
    default:
        return 0;
    }
}
