#include "arithmetic_and_logic_shift_unit.h"
#include "macros.h"

unsigned int alsu(unsigned int num, unsigned int amount, enum ALSUOp op) {
  switch (op) {
  case ALSUOp_SLL:
    return num << amount;
  case ALSUOp_SRL:
    return num >> amount;
  case ALSUOp_SRA:
    if ((int)num < 0) {
      // SRA on signed integers is compiler dependent behavior. Sometimes, it's
      // even undefined behavior. Therefore it's not reliable.
      // Source: https://stackoverflow.com/a/53766752
      return (unsigned int)(~(~(int)num >> amount));
    } else {
      return num >> amount;
    }
  case ALSUOp_ROR:
    // - num >> amount: Make room for the new digits
    // - num & MASK(amount): Isolate the bits to be moved. Let result be RES
    // - RES << (32 - amount): Put the bits in the Most Significant Bits
    // position
    return (num >> amount) | ((num & MASK(amount)) << (32 - amount));
  default:
    return 0;
  }
}
