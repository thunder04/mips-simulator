#ifndef _MICROCODE
#define _MICROCODE

#include "../../multiplexers.h"

// Naming conventions:
//   - m stands for microcode
//   - m and the capitalized letters become the prefix of each enum variant
//   - The name of a variant (excl. the prefix) is in snake_case
// The default behavior is the first enum variant
enum mSequencingKind { mSKseq, mSKdisp, mSKlabel };
enum mAluControl { mACnothing, mACadd, mACsub, mACand, mACor };
enum mAlSuControl { mASCnothing, mASCsll, mASCsrl, mASCsra, mASCror };
enum mExitControl { mECcontinue, mECexit };

enum mMemControl {
  // Do nothing
  mMCnothing,
  // Read the address PC, and save the result to IR and DR
  mMCread_pc,
  // Read the address C, and save the result to DR
  mMCread_c,
  // Write the contents of B to the address C
  mMCwrite_c,
};

enum mRfControl {
  // Do nothing
  mRCnothing,
  // Read the rs and rt fields of IR and save them to A and B, respectively
  mRCread_rs_rt,
  // Read the rs and rd fields of IR and save them to A and B, respectively
  mRCread_rs_rd,
  // Write to rd the contents of C
  mRCwrite_rd_c,
  // Write to rd the contents of D
  mRCwrite_rd_d,
  // Write to rt the contents of C
  mRCwrite_rt_c,
  // Write to rt the contents of D
  mRCwrite_rt_d,
  // Write to rt the contents of DR
  mRCwrite_rt_dr,
  // Write to ra the contents of A
  mRCwrite_ra_a,
  // Write to ra the contents of B
  mRCwrite_ra_b,
  // Write to ra the contents of PC
  mRCwrite_ra_pc,
};

enum mPcWriteControl {
  // Do nothing (PCWrite & PCWriteCond signals is disabled)
  mPWCnothing,
  // Write to PC the result of ALU
  mPWCalu_out,
  // Write to PC if C is zero
  mPWCc_cond,
  // Write to PC the jump address
  mPWCjump_address,
  // Write to PC if C is NOT zero
  mPWCc_not_cond,
  // Write to PC the contents of A
  mPWCa,
  // Write to PC the contents of B
  mPWCb,
};

struct mSequencing {
  enum mSequencingKind kind;
  // Label is actually the index in the microcode
  unsigned int label;
};

struct Microinstruction {
  struct mSequencing sequencing;

  enum mAluControl alu;
  enum ALUSrcASel alu1;
  enum ALUSrcBSel alu2;

  enum mAlSuControl alsu;
  enum ALSUSrcASel alsu1;
  enum ALSUSrcBSel alsu2;

  enum mRfControl rf;
  enum mMemControl mem;
  enum mPcWriteControl pc;

  // Special operation that will exit the simulation, if enabled
  enum mExitControl exit;
};

extern const struct Microinstruction MICROCODE[];

int opcode_to_microcode_idx(int opcode);

#endif
