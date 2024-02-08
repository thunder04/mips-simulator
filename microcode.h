#include "multiplexers.h"
#include "units/arithmetic_and_logic_shift_unit.h"
#include "units/arithmetic_and_logic_unit.h"

// Naming conventions:
//   - m stands for microcode
//   - m and the capitalized letters become the prefix of each enum variant
//   - The name of a variant (excl. the prefix) is in snake_case
// The default behavior is the first enum variant
enum mSequencingKind { mSKseq, mSKdisp, mSKlabel };
enum mAluControl { mACnothing, mACadd, mACsub, mACfunct };
enum mAlSuControl { mASCnothing, mASCsll, mASCsrl, mASCsra, mASCror };
enum mRfControl { mRCnothing, mRCread, mRCwrite_c, mRCwrite_dr };
enum mMemControl { mMCnothing, mMCread_pc, mMCread_c, mMCwrite_c };
enum mPcWriteControl { mPWCnothing, mPWCalu_out, mPWCc_cond, mPWCjump_address };
enum mExitControl { mECcontinue, mECexit };

struct mSequencing {
  enum mSequencingKind kind;
  // Label is actually the index in the microcode
  unsigned int label;
};

struct MicrocodeRow {
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

int opcode_to_microcode_idx(int opcode);
